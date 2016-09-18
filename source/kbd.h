//
// Created by lichevsky on 17.09.16.
//

#ifndef MAIN_EXEC_KBD_H
#define MAIN_EXEC_KBD_H


#include <3ds.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "string_utils.h"

using namespace std;
struct xy{u32 x; u32 y;};
struct KBDBank{
    string A;
    string B;
    string X;
    string Y;
    string id(){
        return A+B+X+Y;
    }
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
    vector<KBDBank> getBanks(){
        return {TOP, TOP_RIGHT, RIGHT, BOTTOM_RIGHT, BOTTOM, BOTTOM_LEFT, LEFT, TOP_LEFT};
    }
};
class KBD {
public:
    KBD(gfxScreen_t, void (*cb)(string), KBDLayout defaultLayout);
    void clock(u32 keysHeld, u32 keysDown, u32 keysUp);
    void setEnaled(bool);
    void registerLayout(u32 key, KBDLayout l);
private:
    bool isEnabled;
    PrintConsole pc;
    u32 keysHeld;
    KBDBank selectedBank;
    KBDBank specialBank;
    bool cpad_held;
    void (*callback)(string);
    void printLayout();
    void printBank(KBDBank , xy , bool);
    KBDLayout selectedLayout();
    KBDLayout defaultLayout;
    KBDBank cpad_to_bank_id();
    map<u32, KBDLayout> layoutMap;
};

#endif //MAIN_EXEC_KBD_H
