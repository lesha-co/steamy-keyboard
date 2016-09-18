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


KBD::KBD(gfxScreen_t display, void (*cb)(string), vector<KBDLayout> layouts) {

    this->layout = 0;
    this->modeSwitchKey = KEY_L;
    this->callback = cb;
    this->layouts = layouts;
    this->isEnabled= true;
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
    // rotating modes if modeSwitchKey pressed

    if(!this->isEnabled) return;
    if(keysDown & this->modeSwitchKey){
        this->layout++;
        if(this->layout >= this->layouts.size()){
            this->layout = 0;
        }

    }
    // determining cpad
    u32 all_cpad = (KEY_CPAD_RIGHT | KEY_CPAD_UP | KEY_CPAD_LEFT | KEY_CPAD_DOWN );
    u32 all_ABXY = (KEY_A | KEY_B | KEY_X | KEY_Y);
    this->cpad_held = (keysHeld & all_cpad)!=0;
    bool abxy_pressed = (keysDown & all_ABXY)!=0 ;
    if(this->cpad_held){
        this->selectedBank = cpad_to_bank_id(keysHeld, keysDown);
    }
    // determining buttons

    if(keysDown || keysUp){
        printLayout();
    }
    if(abxy_pressed){
        KBDBank special = {"\n", " ", " ", " "};
        KBDBank bank = this->cpad_held?
                       selectedLayout().getBank(this->selectedBank):
                       special;
        if(keysDown & KEY_A){
            callback(bank.A);
        }
        else if(keysDown & KEY_B){
            callback(bank.B);
        }
        if(keysDown & KEY_X){
            callback(bank.X);
        }
        if(keysDown & KEY_Y){
            callback(bank.Y);
        }
    }


    return;
}


u8 KBD::cpad_to_bank_id(u32 keysHeld, u32 keysDown) {
    //determines which character bank corresponds to current cpad state
    bool cpad_left  = (keysHeld & KEY_CPAD_LEFT)!=0;
    bool cpad_up    = (keysHeld & KEY_CPAD_UP)!=0;
    bool cpad_right = (keysHeld & KEY_CPAD_RIGHT)!=0;
    bool cpad_down  = (keysHeld & KEY_CPAD_DOWN)!=0;
    if (!cpad_left &&  cpad_up && !cpad_right && !cpad_down){
        return 0;
    }
    if (!cpad_left &&  cpad_up &&  cpad_right && !cpad_down){
        return 1;
    }
    if (!cpad_left && !cpad_up &&  cpad_right && !cpad_down){
        return 2;
    }
    if (!cpad_left && !cpad_up &&  cpad_right &&  cpad_down){
        return 3;
    }
    if (!cpad_left && !cpad_up && !cpad_right &&  cpad_down){
        return 4;
    }
    if ( cpad_left && !cpad_up && !cpad_right &&  cpad_down){
        return 5;
    }
    if ( cpad_left && !cpad_up && !cpad_right && !cpad_down){
        return 6;
    }
    if ( cpad_left &&  cpad_up && !cpad_right && !cpad_down){
        return 7;
    }
    return 9;

}

void KBD::printLayout() {
    // prints kbd UI
    KBDBank specials = {"N",">", " ", " "};
    xy coords = {8,4};
    consoleSelect(&this->pc);
    u8 nbanks = this->selectedLayout().N_BANKS();
    for (u8 i = 0; i < nbanks; ++i) {
        bool selected = (i == this->selectedBank && this->cpad_held);
        printBank(this->selectedLayout().getBank(i), bank_coords[i], selected);
    }
    printBank(specials, coords, !this->cpad_held);
}

void KBD::printBank(KBDBank bank, xy bankCoords, bool selected){
    //subroutine to printing character bank
    string clr = setColor(COLOR_WHITE, COLOR_BLACK);
    if(selected){
        clr = setColor(COLOR_BLACK, COLOR_WHITE);
    }
    std::cout << position(bankCoords.y, bankCoords.x) << clr << " "+bank.X+" " << resetColor();
    std::cout << position(bankCoords.y+1, bankCoords.x) << clr << bank.Y+" "+bank.A<< resetColor();
    std::cout << position(bankCoords.y+2, bankCoords.x) << clr << " "+bank.B+" " << resetColor();
}

KBDLayout KBD::selectedLayout() {
    return this->layouts[this->layout];
}

void KBD::setEnaled(bool isEnabled) {
    this->isEnabled = isEnabled;
}