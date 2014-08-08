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
#include <string.h>
#include "cpu.h"
#include "opcodes.h"

const uint8_t chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

void cpu_init(FILE *t, struct cpu *CPU) {
    uint16_t i = 0;

    memset(CPU, '\0', sizeof(struct cpu));
    CPU->registers.PC = 0x200;

    for (i=0;i<80;i++) {
        CPU->memory[i] = chip8_fontset[i];
    }

    i = 0;

    while (!feof(t)) {
        int fH = fgetc(t);
        if (fH == -1) break;
        CPU->memory[CPU->registers.PC + i] = fH;

        int fL = fgetc(t);
        if (fL == -1) break;
        CPU->memory[CPU->registers.PC + i + 1] = fL;

        i += 2;
    }
}

int cpu_cycle(struct cpu *CPU) {
    int ret = 0;
    uint8_t fH = CPU->memory[CPU->registers.PC];
    uint8_t fL = CPU->memory[CPU->registers.PC + 1];
    uint16_t opcode = (fH << 8) | fL;

    /*printf("0x%.4X: %.4X [I=%.4X] [SP=%.4X] ", CPU->registers.PC, opcode, CPU->registers.I, CPU->registers.SP);*/

    CPU->registers.PC += 2;
    parse_opcode(CPU, opcode);

    return ret;

}
