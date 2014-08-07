/*
This file is part of CHIP-8 VM.

CHIP-8 VM is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CHIP-8 VM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CHIP-8 VM. If not, see <http://www.gnu.org/licenses/>.

*/
#include "keyboard.h"

const char keymap[] = "1234qwerasdfzxcv";

void keyboardDown(unsigned char key, struct cpu *CPU) {
    if (key == 27) {
        CPU->halt = 1;
        return;
    }

    if (key == keymap[0]) CPU->key[0x1] = 1;
    else if (key == keymap[1]) CPU->key[0x2] = 1;
    else if (key == keymap[2]) CPU->key[0x3] = 1;
    else if (key == keymap[3]) CPU->key[0xC] = 1;

    else if (key == keymap[4]) CPU->key[0x4] = 1;
    else if (key == keymap[5]) CPU->key[0x5] = 1;
    else if (key == keymap[6]) CPU->key[0x6] = 1;
    else if (key == keymap[7]) CPU->key[0xD] = 1;

    else if (key == keymap[8]) CPU->key[0x7] = 1;
    else if (key == keymap[9]) CPU->key[0x8] = 1;
    else if (key == keymap[10]) CPU->key[0x9] = 1;
    else if (key == keymap[11]) CPU->key[0xE] = 1;

    else if (key == keymap[12]) CPU->key[0xA] = 1;
    else if (key == keymap[13]) CPU->key[0x0] = 1;
    else if (key == keymap[14]) CPU->key[0xB] = 1;
    else if (key == keymap[15]) CPU->key[0xF] = 1;
}

void keyboardUp(unsigned char key, struct cpu *CPU) {
    if (key == keymap[0]) CPU->key[0x1] = 0;
    else if (key == keymap[1]) CPU->key[0x2] = 0;
    else if (key == keymap[2]) CPU->key[0x3] = 0;
    else if (key == keymap[3]) CPU->key[0xC] = 0;

    else if (key == keymap[4]) CPU->key[0x4] = 0;
    else if (key == keymap[5]) CPU->key[0x5] = 0;
    else if (key == keymap[6]) CPU->key[0x6] = 0;
    else if (key == keymap[7]) CPU->key[0xD] = 0;

    else if (key == keymap[8]) CPU->key[0x7] = 0;
    else if (key == keymap[9]) CPU->key[0x8] = 0;
    else if (key == keymap[10]) CPU->key[0x9] = 0;
    else if (key == keymap[11]) CPU->key[0xE] = 0;

    else if (key == keymap[12]) CPU->key[0xA] = 0;
    else if (key == keymap[13]) CPU->key[0x0] = 0;
    else if (key == keymap[14]) CPU->key[0xB] = 0;
    else if (key == keymap[15]) CPU->key[0xF] = 0;
}
