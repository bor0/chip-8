#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "opcodes.h"

const uint8_t chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void cpu_init(FILE *t, struct cpu *CPU) {
    uint16_t i = 0;

    memset(CPU, '\0', sizeof(struct cpu));
    (*CPU).registers.PC = 0x200;

    for (i=0;i<80;i++) {
        (*CPU).memory[i] = chip8_fontset[i];
    }

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

        if ((*CPU).draw == 1) {
            /* do something here */
            (*CPU).draw = 0;
        }

        if ((*CPU).sound_timer > 0) {
            printf("Beep!\n");
            (*CPU).sound_timer--;
        }
        if ((*CPU).delay_timer > 0) {
            printf("Delay timer");
            (*CPU).delay_timer--;
        }

        (*CPU).registers.PC += 2;
        putchar('\n');
    }
}
