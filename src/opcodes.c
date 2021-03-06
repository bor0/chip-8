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
#include <time.h>

#include "opcodes.h"

void parse_opcode(struct cpu *CPU, uint16_t opcode) {
    uint8_t i, keypress, vx, vy, height, spr;
    uint16_t x, y;

    switch (opcode) {
    case 0x00E0:
        /* Clears the screen. */
        memset(CPU->display, 0, sizeof(CPU->display));
        return;
        break;
    case 0x00EE:
        /* Returns from a subroutine. */
        CPU->registers.SP--;
        if (CPU->registers.SP < 0) {
            printf("Fatal error: Stack underflow\n");
            CPU->halt = 1;
            return;
        }
        CPU->registers.PC = CPU->stack[CPU->registers.SP];
        return;
        break;
    default:
        break;
    }

    switch (opcode & 0xF000) {
    case 0x0000:
        /* Calls RCA 1802 program at address NNN. */
        printf("RCA 1802 call instruction not implemented.\n");
        return;
        break;
    case 0x1000:
        /* Jumps to address NNN. */
        CPU->registers.PC = opcode & 0x0FFF;
        return;
        break;
    case 0x2000:
        /* Calls subroutine at NNN. */
        if (CPU->registers.SP > sizeof(CPU->stack) / sizeof(uint16_t)) {
            printf("Fatal error: Stack overflow\n");
            CPU->halt = 1;
        }
        CPU->stack[CPU->registers.SP] = CPU->registers.PC;
        CPU->registers.SP++;
        CPU->registers.PC = opcode & 0x0FFF;
        return;
        break;
    case 0x3000:
        /* Skips the next instruction if VX equals NN. */
        if (CPU->registers.v[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
            CPU->registers.PC += 2;
        }
        return;
        break;
    case 0x4000:
        /* Skips the next instruction if VX doesn't equal NN. */
        if (CPU->registers.v[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
            CPU->registers.PC += 2;
        }
        return;
        break;
    case 0x6000:
        /* Sets VX to NN. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        return;
        break;
    case 0x7000:
        /* Adds NN to VX. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        return;
        break;
    case 0xA000:
        /* Sets I to the address NNN. */
        CPU->registers.I = opcode & 0x0FFF;
        return;
        break;
    case 0xB000:
        /* Jumps to the address NNN plus V0. */
        CPU->registers.PC = (opcode & 0x0FFF) + CPU->registers.v[0];
        return;
        break;
    case 0xC000:
        /* Sets VX to a random number and NN. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] = (clock() % 255) & (opcode & 0x00FF);
        return;
        break;
    case 0xD000:
        /* Sprites stored in CPU at location in index register (I), maximum 8bits wide.
           Wraps around the screen. If when drawn, clears a pixel, register VF is set to 1
           otherwise it is zero. All drawing is XOR drawing (e.g. it toggles the screen pixels) */
        vx = CPU->registers.v[(opcode & 0x0F00) >> 8];
        vy = CPU->registers.v[(opcode & 0x00F0) >> 4];

        CPU->registers.v[15] = 0;

        height = opcode & 0x000F;
        if (!height) height = 16;

        for (y = 0; y < height; y++) {
            uint8_t wy;

            spr = CPU->memory[CPU->registers.I + y];
            wy = (vy + y) % 32;
            for (x = 0; x < 8; x++) {
                uint8_t wx = (vx + x) % 64;
                if ((spr & 0x80) > 0) {
                    if (CPU->display[wy + wx * 32] == 1) {
                        CPU->registers.v[15] = 1;
                    }
                    CPU->display[wy + wx * 32] ^= 1;
                }
                spr <<= 1;
            }
        }
        return;
        break;
    default:
        break;
    }

    switch (opcode & 0xF00F) {
    case 0x5000:
        /* Skips the next instruction if VX equals VY. */
        if (CPU->registers.v[(opcode & 0x0F00) >> 8] == CPU->registers.v[(opcode & 0x00F0) >> 4]) {
            CPU->registers.PC += 2;
        }
        return;
        break;
    case 0x8000:
        /* Sets VX to the value of VY. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] = CPU->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8001:
        /* Sets VX to VX or VY. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] |= CPU->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8002:
        /* Sets VX to VX and VY. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] &= CPU->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8003:
        /* Sets VX to VX xor VY. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] ^= CPU->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8004:
        /* Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't. */
        vx = CPU->registers.v[(opcode & 0x0F00) >> 8];
        vy = CPU->registers.v[(opcode & 0x00F0) >> 4];

        if ((int)vx + vy > 0xFF) {
            CPU->registers.v[15] = 1;
        } else {
            CPU->registers.v[15] = 0;
        }

        CPU->registers.v[(opcode & 0x0F00) >> 8] += vy;
        return;
        break;
    case 0x8005:
        /* VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't. */
        vx = CPU->registers.v[(opcode & 0x0F00) >> 8];
        vy = CPU->registers.v[(opcode & 0x00F0) >> 4];

        if ((int)vx - vy >= 0) {
            CPU->registers.v[15] = 1;
        } else {
            CPU->registers.v[15] = 0;
        }

        CPU->registers.v[(opcode & 0x0F00) >> 8] -= vy;
        return;
        break;
    case 0x8006:
        /* Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift. */
        CPU->registers.v[15] = CPU->registers.v[(opcode & 0x0F00) >> 8] & 1;
        CPU->registers.v[(opcode & 0x0F00) >> 8] >>= 1;
        return;
        break;
    case 0x8007:
        /* Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't. */
        vx = CPU->registers.v[(opcode & 0x0F00) >> 8];
        vy = CPU->registers.v[(opcode & 0x00F0) >> 4];

        if ((int)vy - vx >= 0) {
            CPU->registers.v[15] = 1;
        } else {
            CPU->registers.v[15] = 0;
        }

        CPU->registers.v[(opcode & 0x0F00) >> 8] = vy - CPU->registers.v[(opcode & 0x0F00) >> 8];
        return;
        break;
    case 0x800E:
        /* Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift. */
        CPU->registers.v[15] = (CPU->registers.v[(opcode & 0x0F00) >> 8] >> 7) & 0x80;
        CPU->registers.v[(opcode & 0x0F00) >> 8] <<= 1;
        return;
        break;
    case 0x9000:
        /* Skips the next instruction if VX doesn't equal VY. */
        if (CPU->registers.v[(opcode & 0x0F00) >> 8] != CPU->registers.v[(opcode & 0x00F0) >> 4]) {
            CPU->registers.PC += 2;
        }
        return;
        break;
    default:
        break;
    }

    switch (opcode & 0xF0FF) {
    case 0xE09E:
        /* Skips the next instruction if the key stored in VX is pressed. */
        vx = CPU->registers.v[(opcode & 0x0F00) >> 8];
        if (CPU->key[vx]) {
            CPU->registers.PC += 2;
        }
        return;
        break;
    case 0xE0A1:
        /* Skips the next instruction if the key stored in VX isn't pressed. */
        vx = CPU->registers.v[(opcode & 0x0F00) >> 8];
        if (!CPU->key[vx]) {
            CPU->registers.PC += 2;
        }
        return;
        break;
    case 0xF007:
        /* Sets VX to the value of the delay timer. */
        CPU->registers.v[(opcode & 0x0F00) >> 8] = CPU->delay_timer;
        return;
        break;
    case 0xF00A:
        /* A key press is awaited, and then stored in VX. */
        for (i = 0, keypress = 0; i < 16; i++) {
            if (CPU->key[i]) {
                CPU->key[i] = 0;
                CPU->registers.v[(opcode & 0x0F00) >> 8] = i;
                keypress = 1;
            }
        }

        /* No keypress, repeat cycle */
        if (!keypress) {
            CPU->registers.PC -= 2;
        }
        return;
        break;
    case 0xF015:
        /* Sets the delay timer to VX. */
        CPU->delay_timer = CPU->registers.v[(opcode & 0x0F00) >> 8];
        return;
        break;
    case 0xF018:
        /* Sets the sound timer to VX. */
        CPU->sound_timer = CPU->registers.v[(opcode & 0x0F00) >> 8];
        return;
        break;
    case 0xF01E:
        /* Adds VX to I. */
        if ((CPU->registers.I += CPU->registers.v[(opcode & 0x0F00) >> 8]) > 0x0FFF) {
            CPU->registers.v[15] = 1;
        } else {
            CPU->registers.v[15] = 0;
        }
        return;
        break;
    case 0xF029:
        /* Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font. */
        CPU->registers.I = CPU->registers.v[(opcode & 0x0F00) >> 8] * 5;
        return;
        break;
    case 0xF033:
        /* Stores the Binary-coded decimal representation of VX, with the most significant
           of three digits at the address in I, the middle digit at I plus 1, and the least
           significant digit at I plus 2. (In other words, take the decimal representation of VX,
           place the hundreds digit in CPU at location in I, the tens digit at location I+1,
           and the ones digit at location I+2.) */
        vx = CPU->registers.v[(opcode & 0x0F00) >> 8];
        CPU->memory[CPU->registers.I] = (vx / 100) % 10;
        CPU->memory[CPU->registers.I + 1] = (vx / 10) % 10;
        CPU->memory[CPU->registers.I + 2] = vx % 10;
        return;
        break;
    case 0xF055:
        /* Stores V0 to VX in CPU starting at address I. */
        for (i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
             CPU->memory[CPU->registers.I++] = CPU->registers.v[i];
        }
        return;
        break;
    case 0xF065:
        /* Fills V0 to VX with values from CPU starting at address I. */
        for (i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
            CPU->registers.v[i] = CPU->memory[CPU->registers.I++];
        }
        return;
        break;
    default:
        break;
    }

    printf("Unknown opcode.\n");
    CPU->halt = 1;
}
