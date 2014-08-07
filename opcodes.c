#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cpu.h"
#include "opcodes.h"

void NI(char *str) {
    printf(" [%s] This instruction is not implemented.", str);
}

void parse_opcode(struct cpu *CPU, uint16_t opcode) {
    int i;

    printf("[PC = %.4X] [OPCODE: %.4X] ", (*CPU).registers.PC, opcode);

    if (opcode == 0x00E0) {
        /* Clears the screen. */
        memset((*CPU).display, 0, sizeof((*CPU).display));
        (*CPU).draw = 1;
    }

    else if (opcode == 0x00EE) {
        /* Returns from a subroutine. */
        (*CPU).SP--;
        if ((*CPU).SP < 0) {
            printf("Fatal error: Stack underflow");
            (*CPU).halt = 1;
            return;
        }
        (*CPU).registers.PC = (*CPU).stack[(*CPU).SP];
    }

    else if ((opcode & 0xF000) == 0x0000) {
        /* Calls RCA 1802 program at address NNN. */
        NI("Calls RCA 1802 program at address NNN.");
    }

    else if ((opcode & 0xF000) == 0x1000) {
        /* Jumps to address NNN. */
        (*CPU).registers.PC = opcode & 0x0FFF;
    }

    else if ((opcode & 0xF000) == 0x2000) {
        /* Calls subroutine at NNN. */
        (*CPU).stack[(*CPU).SP] = (*CPU).registers.PC;
        if ((*CPU).SP > sizeof((*CPU).stack)) {
            printf("Fatal error: Stack overflow");
            (*CPU).halt = 1;
        }
        (*CPU).SP++;
        (*CPU).registers.PC = (opcode & 0x0FFF);
    }

    else if ((opcode & 0xF000) == 0x3000) {
        /* Skips the next instruction if VX equals NN. */
        if ((*CPU).registers.v[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
            (*CPU).registers.PC += 2;
        }
    }

    else if ((opcode & 0xF000) == 0x4000) {
        /* Skips the next instruction if VX doesn't equal NN. */
        if ((*CPU).registers.v[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
            (*CPU).registers.PC += 2;
        }
    }

    else if ((opcode & 0xF00F) == 0x5000) {
        /* Skips the next instruction if VX equals VY. */
        if ((*CPU).registers.v[(opcode & 0x0F00) >> 8] == (*CPU).registers.v[(opcode & 0x00F0) >> 4]) {
            (*CPU).registers.PC += 2;
        }
    }

    else if ((opcode & 0xF000) == 0x6000) {
        /* Sets VX to NN. */
        (*CPU).registers.v[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
    }

    else if ((opcode & 0xF000) == 0x7000) {
        /* Adds NN to VX. */
        (*CPU).registers.v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    }

    else if ((opcode & 0xF00F) == 0x8000) {
        /* Sets VX to the value of VY. */
        (*CPU).registers.v[(opcode & 0x0F00) >> 8] = (*CPU).registers.v[(opcode & 0x0F00) >> 4];
    }

    else if ((opcode & 0xF00F) == 0x8001) {
        /* Sets VX to VX or VY. */
        (*CPU).registers.v[(opcode & 0x0F00) >> 8] |= (*CPU).registers.v[(opcode & 0x0F00) >> 4];
    }

    else if ((opcode & 0xF00F) == 0x8002) {
        /* Sets VX to VX and VY. */
        (*CPU).registers.v[(opcode & 0x0F00) >> 8] &= (*CPU).registers.v[(opcode & 0x0F00) >> 4];
    }

    else if ((opcode & 0xF00F) == 0x8003) {
        /* Sets VX to VX xor VY. */
        (*CPU).registers.v[(opcode & 0x0F00) >> 8] ^= (*CPU).registers.v[(opcode & 0x0F00) >> 4];
    }

    else if ((opcode & 0xF00F) == 0x8004) {
        /* Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't. */
        int vy = (*CPU).registers.v[(opcode & 0x0F00) >> 8];
        int vx = (*CPU).registers.v[(opcode & 0x00F0) >> 4];

        if (vx + vy > 0xFFFF) {
            (*CPU).registers.v[15] = 1;
        } else {
            (*CPU).registers.v[15] = 0;
        }

        (*CPU).registers.v[(opcode & 0x0F00) >> 8] += vy;
    }

    else if ((opcode & 0xF00F) == 0x8005) {
        /* VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't. */
        int vy = (*CPU).registers.v[(opcode & 0x0F00) >> 8];
        int vx = (*CPU).registers.v[(opcode & 0x00F0) >> 4];

        if (vx - vy < 0) {
            (*CPU).registers.v[15] = 0;
        } else {
            (*CPU).registers.v[15] = 1;
        }

        (*CPU).registers.v[(opcode & 0x00F0) >> 4] -= vy;
    }

    else if ((opcode & 0xF00F) == 0x8006) {
        /* Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift. */
        (*CPU).registers.v[(opcode & 0x00F0 >> 4)] = ((*CPU).registers.v[(opcode & 0x0F00 >> 8)] & 1);
        (*CPU).registers.v[(opcode & 0x0F00 >> 8)] >>= 1;
    }

    else if ((opcode & 0xF00F) == 0x8007) {
        /* Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't. */
        int vx = (*CPU).registers.v[(opcode & 0x0F00) >> 8];
        int vy = (*CPU).registers.v[(opcode & 0x00F0) >> 4];

        if (vy - vx < 0) {
            (*CPU).registers.v[15] = 0;
        } else {
            (*CPU).registers.v[15] = 1;
        }

        (*CPU).registers.v[(opcode & 0x0F00) >> 8] = vy - (*CPU).registers.v[(opcode & 0x0F00) >> 8];
    }

    else if ((opcode & 0xF00F) == 0x800E) {
        /* Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift. */
        (*CPU).registers.v[(opcode & 0x00F0 >> 4)] = ((*CPU).registers.v[(opcode & 0x0F00 >> 8)] >> 15);
        (*CPU).registers.v[(opcode & 0x0F00 >> 8)] <<= 1;
    }

    else if ((opcode & 0xF00F) == 0x9000) {
        /* Skips the next instruction if VX doesn't equal VY. */
        if ((*CPU).registers.v[(opcode & 0x0F00) >> 8] != (*CPU).registers.v[(opcode & 0x00F0) >> 4]) {
            (*CPU).registers.PC += 2;
        }
    }

    else if ((opcode & 0xF000) == 0xA000) {
        /* Sets I to the address NNN. */
        (*CPU).registers.I = opcode & 0x0FFF;
    }

    else if ((opcode & 0xF000) == 0xB000) {
        /* Jumps to the address NNN plus V0. */
        (*CPU).registers.PC = opcode & 0x0FFF + (*CPU).registers.v[0];
    }

    else if ((opcode & 0xF000) == 0xC000) {
        /* Sets VX to a random number and NN. */
        (*CPU).registers.v[(opcode & 0x0F00) >> 8] = ((opcode & 0x00FF) & clock());
    }

    else if ((opcode & 0xF000) == 0xD000) {
        /* Sprites stored in CPU at location in index register (I), maximum 8bits wide.
           Wraps around the screen. If when drawn, clears a pixel, register VF is set to 1
           otherwise it is zero. All drawing is XOR drawing (e.g. it toggles the screen pixels) */
        uint8_t vx = (*CPU).registers.v[(opcode & 0x0F00) >> 8];
        uint8_t vy = (*CPU).registers.v[(opcode & 0x00F0) >> 4];
        uint8_t height = opcode & 0x000F;
        uint8_t pixel;
        int yline, xline;

        (*CPU).registers.v[0xF] = 0;
        for (yline=0;yline<height;yline++) {
            pixel = (*CPU).memory[(*CPU).registers.I + yline];
            for (xline=0;xline<8;xline++) {
                if ((pixel & (0x80 >> xline)) != 0) {
                    if((*CPU).display[(vx + xline + ((vy + yline) * 64))] == 1) {
                        (*CPU).registers.v[0xF] = 1;
                    }
                    (*CPU).display[vx + xline + ((vy + yline) * 64)] ^= 1;
                }
            }
        }

        (*CPU).draw = 1;
    }

    else if ((opcode & 0xF0FF) == 0xE09E) {
        /* Skips the next instruction if the key stored in VX is pressed. */
        NI("Skips the next instruction if the key stored in VX is pressed.");
    }

    else if ((opcode & 0xF0FF) == 0xE0A1) {
        /* Skips the next instruction if the key stored in VX isn't pressed. */
        NI("Skips the next instruction if the key stored in VX isn't pressed.");
    }

    else if ((opcode & 0xF0FF) == 0xF007) {
        /* Sets VX to the value of the delay timer. */
        (*CPU).registers.v[(opcode & 0xF0FF) >> 8] = (*CPU).delay_timer;
    }

    else if ((opcode & 0xF0FF) == 0xF00A) {
        /* A key press is awaited, and then stored in VX. */
        NI("A key press is awaited, and then stored in VX.");
    }

    else if ((opcode & 0xF0FF) == 0xF015) {
        /* Sets the delay timer to VX. */
        (*CPU).delay_timer = (*CPU).registers.v[(opcode & 0xF0FF) >> 8];
    }

    else if ((opcode & 0xF0FF) == 0xF018) {
        /* Sets the sound timer to VX. */
        (*CPU).sound_timer = (*CPU).registers.v[(opcode & 0xF0FF) >> 8];
    }

    else if ((opcode & 0xF0FF) == 0xF01E) {
        /* Adds VX to I. */
        (*CPU).registers.I += (*CPU).registers.v[(opcode & 0x0F00) >> 8];
    }

    else if ((opcode & 0xF0FF) == 0xF029) {
        /* Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font. */
        (*CPU).registers.I = (*CPU).memory[(*CPU).registers.v[(opcode & 0xF0FF) >> 8]];
    }

    else if ((opcode & 0xF0FF) == 0xF033) {
        /* Stores the Binary-coded decimal representation of VX, with the most significant
           of three digits at the address in I, the middle digit at I plus 1, and the least
           significant digit at I plus 2. (In other words, take the decimal representation of VX,
           place the hundreds digit in CPU at location in I, the tens digit at location I+1,
           and the ones digit at location I+2.) */
        int vx = (*CPU).registers.v[(opcode & 0x0F00) >> 8];
        (*CPU).memory[(*CPU).registers.I] = vx / 100;
        (*CPU).memory[(*CPU).registers.I] = vx / 10 % 10;
        (*CPU).memory[(*CPU).registers.I] = vx % 10;
    }

    else if ((opcode & 0xF0FF) == 0xF055) {
        /* Stores V0 to VX in CPU starting at address I. */
        for (i=0;i<16;i++) {
             (*CPU).memory[(*CPU).registers.I + i] = (*CPU).registers.v[i];
        }
    }

    else if ((opcode & 0xF0FF) == 0xF065) {
        /* Fills V0 to VX with values from CPU starting at address I. */
        for (i=0;i<16;i++) {
            (*CPU).registers.v[i] = (*CPU).memory[(*CPU).registers.I + i];
        }
    }

    else {
        NI("Unknown opcode");
    }
}
