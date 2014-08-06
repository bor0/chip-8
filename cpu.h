#ifndef CPU_H
#define CPU_H
#include <stdint.h>

struct cpu {
    struct {
        /* 16 8-bit data registers named from V0 to VF. The VF register doubles as a carry flag. */
        uint8_t v[16];

        /* The address register is 16 bits wide and is used with several opcodes that involve memory operations. */
        uint16_t I;

        /* Program counter */
        uint16_t PC;
    } registers;

    /* CHIP-8's memory addresses range from 200h to FFFh, making for 3,584 bytes. */
    uint8_t memory[0x0FFF];

    /* Display resolution is 64×32 pixels, and color is monochrome. */
    uint8_t display[64][32];

    /* The original 1802 version allocated 48 bytes for up to 12 levels of nesting; modern implementations normally have at least 16 levels. */
    uint16_t stack[16];

    /* Stack pointer */
    uint16_t SP;

    /* Halt variable */
    int halt;

};

void cpu_init(FILE *, struct cpu *);
void cpu_loop(struct cpu *);
#endif
