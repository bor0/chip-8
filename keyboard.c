#include "keyboard.h"

void keyboardDown(unsigned char key, struct cpu *CPU) {
    if (key == 27) {
        exit(0);
    }

    if (key == keymap[0]) (*CPU).key[0x1] = 1;
    else if (key == keymap[1]) (*CPU).key[0x2] = 1;
    else if (key == keymap[2]) (*CPU).key[0x3] = 1;
    else if (key == keymap[3]) (*CPU).key[0xC] = 1;

    else if (key == keymap[4]) (*CPU).key[0x4] = 1;
    else if (key == keymap[5]) (*CPU).key[0x5] = 1;
    else if (key == keymap[6]) (*CPU).key[0x6] = 1;
    else if (key == keymap[7]) (*CPU).key[0xD] = 1;

    else if (key == keymap[8]) (*CPU).key[0x7] = 1;
    else if (key == keymap[9]) (*CPU).key[0x8] = 1;
    else if (key == keymap[10]) (*CPU).key[0x9] = 1;
    else if (key == keymap[11]) (*CPU).key[0xE] = 1;

    else if (key == keymap[12]) (*CPU).key[0xA] = 1;
    else if (key == keymap[13]) (*CPU).key[0x0] = 1;
    else if (key == keymap[14]) (*CPU).key[0xB] = 1;
    else if (key == keymap[15]) (*CPU).key[0xF] = 1;
}

void keyboardUp(unsigned char key, struct cpu *CPU) {
    if (key == keymap[0]) (*CPU).key[0x1] = 0;
    else if (key == keymap[1]) (*CPU).key[0x2] = 0;
    else if (key == keymap[2]) (*CPU).key[0x3] = 0;
    else if (key == keymap[3]) (*CPU).key[0xC] = 0;

    else if (key == keymap[4]) (*CPU).key[0x4] = 0;
    else if (key == keymap[5]) (*CPU).key[0x5] = 0;
    else if (key == keymap[6]) (*CPU).key[0x6] = 0;
    else if (key == keymap[7]) (*CPU).key[0xD] = 0;

    else if (key == keymap[8]) (*CPU).key[0x7] = 0;
    else if (key == keymap[9]) (*CPU).key[0x8] = 0;
    else if (key == keymap[10]) (*CPU).key[0x9] = 0;
    else if (key == keymap[11]) (*CPU).key[0xE] = 0;

    else if (key == keymap[12]) (*CPU).key[0xA] = 0;
    else if (key == keymap[13]) (*CPU).key[0x0] = 0;
    else if (key == keymap[14]) (*CPU).key[0xB] = 0;
    else if (key == keymap[15]) (*CPU).key[0xF] = 0;
}
