#include <stdio.h>
#include <string.h>
#include "opcodes.h"
#include "cpu.h"

int main(int argc, char *argv[]) {
    struct cpu CPU;

    FILE *f;

    if (argc != 2) {
        printf("usage: %s <game file>\n", argv[0]);
        return 0;
    }

    f = fopen(argv[1], "rb");

    if (!f) {
        printf("cannot open file %s\n", argv[0]);
        return 0;
    }

    memset(&CPU, '\0', sizeof(CPU));

    cpu_init(f, &CPU);

    fclose(f);

    cpu_loop(&CPU);

    return 0;
}
