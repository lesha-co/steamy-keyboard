//
// Created by lichevsky on 17.09.16.
//


#include "kbd.h"
using namespace std;
xy bank_coords[8] {
    {8,0},
    {16,0},
    {16,4},
    {16,8},
    {8,8},
    {0,8},
    {0,4},
    {0,0}
};


KBD::KBD(gfxScreen_t display, void (*cb)(string), KBDLayout defaultLayout) {

    this->defaultLayout = defaultLayout;
    this->callback = cb;
    this->isEnabled= true;
    this->specialBank = {"\n", " ", " ", " "};
    consoleInit(display, &this->pc);
    printLayout();
}

void KBD::clock(u32 keysHeld, u32 keysDown, u32 keysUp) {
    /**
     * determines current state of keyboard and if needed,
     * calls the callback function with character to output
     * @param keysHeld: result of hidKeysHeld() function for current frame
     * @param keysDown: result of hidKeysDown() function for current frame
     * @param keysUp: result of hidKeysUp() function for current frame
     */
    this->keysHeld = keysHeld;
    // rotating modes if modeSwitchKey pressed

    if(!this->isEnabled) return;
    u32 all_cpad = (KEY_CPAD_RIGHT | KEY_CPAD_UP | KEY_CPAD_LEFT | KEY_CPAD_DOWN );
    u32 all_ABXY = (KEY_A | KEY_B | KEY_X | KEY_Y);
    this->cpad_held = (keysHeld & all_cpad)!=0;
    bool abxy_pressed = (keysDown & all_ABXY)!=0 ;

    this->selectedBank = cpad_to_bank_id();

    // determining buttons

    if(keysDown || keysUp){
        printLayout();
    }
    if(abxy_pressed){
        if(keysDown & KEY_A){
            callback(selectedBank.A);
        }
        else if(keysDown & KEY_B){
            callback(selectedBank.B);
        }
        if(keysDown & KEY_X){
            callback(selectedBank.X);
        }
        if(keysDown & KEY_Y){
            callback(selectedBank.Y);
        }
    }
    return;
}

KBDBank KBD::cpad_to_bank_id() {
    //determines which character bank corresponds to current cpad state
    bool cpad_left  = (this->keysHeld & KEY_CPAD_LEFT)!=0;
    bool cpad_up    = (this->keysHeld & KEY_CPAD_UP)!=0;
    bool cpad_right = (this->keysHeld & KEY_CPAD_RIGHT)!=0;
    bool cpad_down  = (this->keysHeld & KEY_CPAD_DOWN)!=0;
    if (!cpad_left &&  cpad_up && !cpad_right && !cpad_down){
        return this->selectedLayout().TOP;
    }
    if (!cpad_left &&  cpad_up &&  cpad_right && !cpad_down){
        return this->selectedLayout().TOP_RIGHT;
    }
    if (!cpad_left && !cpad_up &&  cpad_right && !cpad_down){
        return this->selectedLayout().RIGHT;
    }
    if (!cpad_left && !cpad_up &&  cpad_right &&  cpad_down){
        return this->selectedLayout().BOTTOM_RIGHT;
    }
    if (!cpad_left && !cpad_up && !cpad_right &&  cpad_down){
        return this->selectedLayout().BOTTOM;
    }
    if ( cpad_left && !cpad_up && !cpad_right &&  cpad_down){
        return this->selectedLayout().BOTTOM_LEFT;
    }
    if ( cpad_left && !cpad_up && !cpad_right && !cpad_down){
        return this->selectedLayout().LEFT;
    }
    if ( cpad_left &&  cpad_up && !cpad_right && !cpad_down){
        return this->selectedLayout().LEFT;
    }
    return this->specialBank;

}

void KBD::printLayout() {
    // prints kbd UI
    KBDBank specials = {"N",">", " ", " "};
    xy coords = {8,4};
    consoleSelect(&this->pc);
    vector<KBDBank> banks = this->selectedLayout().getBanks();

    for (u8 i = 0; i < banks.size(); ++i) {
        bool selected = (banks[i].id() == this->selectedBank.id() && this->cpad_held);
        printBank(banks[i], bank_coords[i], selected);
    }
    printBank(specials, coords, !this->cpad_held);
}

void KBD::printBank(KBDBank bank, xy bankCoords, bool selected){
    //subroutine to printing character bank
    string clr = setColor(COLOR_WHITE, COLOR_BLACK);
    if(selected){
        clr = setColor(COLOR_BLACK, COLOR_WHITE);
    }
    cout << position(bankCoords.y, bankCoords.x) << clr << " "+bank.X+" " << resetColor();
    cout << position(bankCoords.y+1, bankCoords.x) << clr << bank.Y+" "+bank.A<< resetColor();
    cout << position(bankCoords.y+2, bankCoords.x) << clr << " "+bank.B+" " << resetColor();
}

KBDLayout KBD::selectedLayout() {
    // determines which layout is currently active
    vector<u32> possibleKeys = {KEY_L, KEY_R};
    for(u32 key : possibleKeys){
        if(this->keysHeld & key){
            if(this->layoutMap.find(key) != this->layoutMap.end()){
                return this->layoutMap[key];
            }
        }

    }
    return this->defaultLayout;
}

void KBD::setEnaled(bool isEnabled) {
    this->isEnabled = isEnabled;
}

void KBD::registerLayout(u32 key, KBDLayout l) {
    // adds layout to possible layouts
    if(this->layoutMap.find(key) == this->layoutMap.end()){
        this->layoutMap[key] =  l;
    }
}
