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
    consoleInit(display, &this->pc);
    printLayout();
}

void KBD::clock(u32 keysHeld, u32 keysDown, u32 keysUp) {
    /**
     * determines current state of keyboard and if needed,
     * calls the callback function with character to output
     * @param keysHeld: result of hidKeysHeld() function for current frame
     * @param keysDown: result of hidKeysDown() function for current frame
     */
    // rotating modes if modeSwitchKey pressed

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
        u8 cell = abxy_to_cell_id(keysHeld, keysDown);
        if(this->cpad_held){
            string s = selectedLayout().getStr(this->selectedBank , cell);
            callback(s);
        } else {
            string special[4] = {"\n", " ", " ", " "};
            callback(special[cell]);
        }
    }


    return;
}

u8 KBD::abxy_to_cell_id(u32 keysHeld, u32 keysDown) {
    if(keysDown & KEY_A){
        return 0;
    }
    if(keysDown & KEY_B){
        return 1;
    }
    if(keysDown & KEY_X){
        return 2;
    }
    if(keysDown & KEY_Y){
        return 3;
    }
    return 0;
}
u8 KBD::cpad_to_bank_id(u32 keysHeld, u32 keysDown) {
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
    return 0;

}

void KBD::printLayout() {
    vector<string> specials = {"N",">", " ", " "};
    xy coords = {8,4};
    consoleSelect(&this->pc);
    for (u8 i = 0; i < KBDLayout::N_BANKS; ++i) {
        bool selected = (i == this->selectedBank && this->cpad_held);
        printBank(this->selectedLayout().getBank(i), bank_coords[i], selected);
    }
    printBank(specials, coords, !this->cpad_held);
}

void KBD::printBank(vector<string> bank, xy bankCoords, bool selected){
    string clr = setColor(COLOR_WHITE, COLOR_BLACK);
    if(selected){
        clr = setColor(COLOR_BLACK, COLOR_WHITE);
    }
    std::cout << position(bankCoords.y, bankCoords.x) << clr << " "+bank[2]+" " << resetColor();
    std::cout << position(bankCoords.y+1, bankCoords.x) << clr << bank[3]+" "+bank[0]<< resetColor();
    std::cout << position(bankCoords.y+2, bankCoords.x) << clr << " "+bank[1]+" " << resetColor();

}

KBDLayout KBD::selectedLayout() {
    return this->layouts[this->layout];
}

KBDLayout::KBDLayout(vector<string> v) {
    this->v = v;
}

string KBDLayout::getStr(u8 bank, u8 cell) {
    return this->v[bank*N_CELLS+cell];
}

vector<string> KBDLayout::getBank(u8 bank) {
    vector<string> slice;
    for (int cell = 0; cell < N_CELLS; ++cell) {
        slice.push_back(this->v[bank*N_CELLS+cell]);
    }
    return slice;
}
