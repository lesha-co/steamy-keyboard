//
// Created by lichevsky on 03.09.16.
//

#include "string_utils.h"

using namespace std;


string colorize(int foreground, int background, string text){
    std::ostringstream stringStream;
    stringStream << setColor(foreground, background) << text << resetColor();
    return stringStream.str();
}

string setColor(int foreground, int background){
    std::ostringstream stringStream;
    stringStream << "\x1b[4" << background << ";3" << foreground << "m";
    return stringStream.str();
}
string resetColor(){
    return "\x1b[0m";
}

string colorize(int foreground, string text){
    return colorize(foreground, COLOR_BLACK, text);
}
string position(u32 line, u32 column){
    std::ostringstream stringStream;
    stringStream << "\x1b[" << line << ";" << column << "H";
    return stringStream.str();
}