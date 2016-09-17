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

class KBDLayout{
public:
    KBDLayout(vector<string> v);
    string getStr(u8, u8);
    vector<string> getBank(u8);
    static const u8 N_BANKS = 8;
    static const u8 N_CELLS = 4;
private:
    vector<string> v;
};

class KBD {
public:
    KBD(gfxScreen_t, void (*cb)(string), vector<KBDLayout> layouts);
    void clock(u32 keysHeld, u32 keysDown, u32 keysUp);

private:
    vector<KBDLayout> layouts;
    PrintConsole pc;
    size_t layout;
    u32 modeSwitchKey;
    u8 selectedBank;
    bool cpad_held;
    void (*callback)(string);
    void printLayout();
    void printBank(vector<string> , xy , bool);
    KBDLayout selectedLayout();
    u8 cpad_to_bank_id(u32 keysHeld, u32 keysDown);
    u8 abxy_to_cell_id(u32 keysHeld, u32 keysDown);
};





#endif //MAIN_EXEC_KBD_H
