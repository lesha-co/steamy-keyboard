//
// Created by lichevsky on 03.09.16.
//

#ifndef FONT_STRING_UTILS_H
#define FONT_STRING_UTILS_H

#include <stdlib.h>
#include <string>
#include <sstream>

#include <3ds.h>

const int COLOR_BLACK = 0;
const int COLOR_RED = 1;
const int COLOR_GREEN = 2;
const int COLOR_YELLOW = 3;
const int COLOR_BLUE = 4;
const int COLOR_MAGENTA = 5;
const int COLOR_CYAN = 6;
const int COLOR_WHITE = 7;
using  namespace std;
string colorize(int foreground, int background, string text);
string colorize(int foreground, string text);
string setColor(int foreground, int background);
string resetColor();
string position(u32 line, u32 column);
#endif //FONT_STRING_UTILS_H
