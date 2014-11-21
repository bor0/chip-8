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
#include <stdio.h>
#include "opcodes.h"
#include "cpu.h"
#include "chip8_sdl.h"

int main(int argc, char *argv[])
{
    struct cpu CPU;

    if (argc != 2) {
        printf("usage: %s <game file>\n", argv[0]);
        return 1;
    }

    cpu_load(&CPU, argv[1]);

    cpu_SDL_loop(&CPU);

    return 0;
}
