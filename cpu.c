#include <stdio.h>
#include "cpu.h"
#include "opcodes.h"

void cpu_init(FILE *t, struct cpu *CPU) {
    int i = 0;
    (*CPU).registers.PC = 0x200;

    while (!feof(t)) {
        int fH = fgetc(t);
        if (fH == -1) break;
        (*CPU).memory[(*CPU).registers.PC + i] = fH;

        int fL = fgetc(t);
        if (fL == -1) break;
        (*CPU).memory[(*CPU).registers.PC + i + 1] = fL;

        i += 2;
    }
}

void cpu_loop(struct cpu *CPU) {
    while (!(*CPU).halt) {
        uint8_t fH = (*CPU).memory[(*CPU).registers.PC];
        uint8_t fL = (*CPU).memory[(*CPU).registers.PC + 1];
        uint16_t opcode = (fH << 8) | fL;

        parse_opcode(CPU, opcode);

        (*CPU).registers.PC += 2;
        putchar('\n');
    }
}
