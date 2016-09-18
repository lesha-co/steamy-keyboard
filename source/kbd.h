//
// Created by lichevsky on 17.09.16.
//

#ifndef MAIN_EXEC_KBD_H
#define MAIN_EXEC_KBD_H


#include <3ds.h>
#include <string>
#include <vector>
#include <iostream>
#include "string_utils.h"

using namespace std;
struct xy{u32 x; u32 y;};
struct KBDBank{
    string A;
    string B;
    string X;
    string Y;
};

struct KBDLayout{
    KBDBank TOP;
    KBDBank TOP_RIGHT;
    KBDBank RIGHT;
    KBDBank BOTTOM_RIGHT;
    KBDBank BOTTOM;
    KBDBank BOTTOM_LEFT;
    KBDBank LEFT;
    KBDBank TOP_LEFT;
    KBDBank getBank(u8 i){
        KBDBank banks[8] = {
            TOP, TOP_RIGHT, RIGHT, BOTTOM_RIGHT, BOTTOM, BOTTOM_LEFT, LEFT, TOP_LEFT
        };
        return banks[i];
    }
    u8 N_BANKS(){
        return 8;
    }
};
class KBD {
public:
    KBD(gfxScreen_t, void (*cb)(string), vector<KBDLayout> layouts);
    void clock(u32 keysHeld, u32 keysDown, u32 keysUp);
    void setEnaled(bool);
private:
    bool isEnabled;
    vector<KBDLayout> layouts;
    PrintConsole pc;
    size_t layout;
    u32 modeSwitchKey;
    u8 selectedBank;
    bool cpad_held;
    void (*callback)(string);
    void printLayout();
    void printBank(KBDBank , xy , bool);
    KBDLayout selectedLayout();
    u8 cpad_to_bank_id(u32 keysHeld, u32 keysDown);
};





#endif //MAIN_EXEC_KBD_H
