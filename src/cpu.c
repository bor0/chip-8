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
#include <string.h>
#include <stdlib.h>
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

void cpu_load(struct cpu *CPU, char *file)
{
    FILE *f = fopen(file, "rb");

    if (!f) {
        printf("cannot open file %s\n", file);
        exit(2);
    }

    cpu_init(CPU, f);
    CPU->program_name = file;

    fclose(f);
}

void cpu_init(struct cpu *CPU, FILE *t)
{
    uint16_t i;

    memset(CPU, '\0', sizeof(struct cpu));
    CPU->registers.PC = 0x200;

    for (i = 0; i < 80; i++) {
        CPU->memory[i] = chip8_fontset[i];
    }

    i = 0;

    while (!feof(t) && i < 0xFFF - 0x200) {
        int fH, fL;

        fH = fgetc(t);
        if (fH == -1) break;
        CPU->memory[CPU->registers.PC + i] = fH;

        fL = fgetc(t);
        if (fL == -1) break;
        CPU->memory[CPU->registers.PC + i + 1] = fL;

        i += 2;
    }
}

void cpu_reset(struct cpu *CPU)
{
    cpu_load(CPU, CPU->program_name);
}

uint16_t calc_opcode(struct cpu *CPU)
{
    uint8_t fH = CPU->memory[CPU->registers.PC];
    uint8_t fL = CPU->memory[CPU->registers.PC + 1];
    return (fH << 8) | fL;
}

void cpu_cycle(struct cpu *CPU)
{
    uint16_t opcode = calc_opcode(CPU);
    CPU->registers.PC += 2;
    parse_opcode(CPU, opcode);
}
