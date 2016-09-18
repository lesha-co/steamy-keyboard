#include <3ds.h>
#include <cstdio>
#include "kbd.h"

PrintConsole printConsole;
// just printing whatever they send to us
void callback(string s){
    consoleSelect(&printConsole);
    printf(s.c_str());
}

int main(int argc, char **argv)
{
    gfxInitDefault();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// run once begin
    ///
    consoleInit(GFX_TOP, &printConsole);
    // here I am initializing layouts
    KBDLayout layout(
        {
            {"c", "d", "b", "a"},
            {"g", "h", "f", "e"},
            {"k", "l", "j", "i"},
            {"o", "p", "n", "m"},
            {"s", "t", "r", "q"},
            {"w", "x", "v", "u"},
            {"?", "!", "z", "y"},
            {"@", "-", ":", "/"}

        });
    KBDLayout layout_caps(
        {
            {"C", "D", "B", "A"},
            {"G", "H", "F", "E"},
            {"K", "L", "J", "I"},
            {"O", "P", "N", "M"},
            {"S", "T", "R", "Q"},
            {"W", "X", "V", "U"},
            {"?", "!", "Z", "Y"},
            {"&", "_","\\", ";"}
        }
    );
    KBDLayout layout_digits(
        {
            {"3", "4", "2", "1"},
            {"7", "8", "6", "5"},
            {"*", "+", "0", "9"},
            {"$", "`", ".", "."},
            {"~", "|","\"", "'"},
            {"%", "^", "#", "="},
            {"[", "]", ">", "<"},
            {"(", ")", "}", "{"}
        }
    );
    // creating kbd instance and telling it to send all input to callback function
    KBD kbd(GFX_BOTTOM, callback, layout);
    kbd.registerLayout(KEY_L, layout_caps);
    kbd.registerLayout(KEY_R, layout_digits);

    ///
    /// run once end
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (aptMainLoop())
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// run loop begin
        ///
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        u32 kUp = hidKeysUp();
        // this method analyzes inputs, redraws kbd ui and if needed, sends strings to callback
        kbd.clock(kHeld, kDown, kUp);
        // break in order to return to hbmenu
        if (kDown & KEY_START) break;

        ///
        /// run loop end
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();
    }

    // Exit services
    gfxExit();

    return 0;
}
