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
#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdlib.h>
#include "cpu.h"

const char keymap[] = "1234qwerasdfzxcv";

void keyboardUp(unsigned char, struct cpu *);
void keyboardDown(unsigned char, struct cpu *);
#endif
