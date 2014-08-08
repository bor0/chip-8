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
#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdio.h>

struct cpu {
    struct {
        /* 16 8-bit data registers named from V0 to VF. The VF register doubles as a carry flag. */
        uint8_t v[16];

        /* The address register is 16 bits wide and is used with several opcodes that involve memory operations. */
        uint16_t I;

        /* Program counter. */
        uint16_t PC;

        /* Stack pointer. */
        uint16_t SP;
    } registers;

    /* CHIP-8's memory addresses range from 200h to FFFh, making for 3,584 bytes. */
    uint8_t memory[0x0FFF];

    /* Display resolution is 64x32 pixels, and color is monochrome. */
    uint8_t display[2048];

    /* The original 1802 version allocated 48 bytes for up to 12 levels of nesting; modern implementations normally have at least 16 levels. */
    uint16_t stack[16];

    /* Key presses, 1 if pressed, 0 if not. */
    uint8_t key[16];

    /* Halt variable. */
    uint8_t halt;

    /* Timers. */
    uint8_t sound_timer, delay_timer;
};

void cpu_init(FILE *, struct cpu *);
int cpu_cycle(struct cpu *);
#endif
