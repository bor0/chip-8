jump    start
.ascii  "GREET.ASM, Copyright 2014 Boro Sitnikovski"
start:
HALO:
    load    v1, 0
    load    v2, 5
    load    i, l_H
    draw    v1, v2, 5
    load    v1, 6
    load    v2, 5
    load    i, l_A
    draw    v1, v2, 5
    load    v1, 12
    load    v2, 5
    load    i, l_L
    draw    v1, v2, 5
    load    v1, 18
    load    v2, 5
    load    i, l_O
    draw    v1, v2, 5
FROM:
    load    v1, 30
    load    v2, 5
    load    i, l_F
    draw    v1, v2, 5
    load    v1, 36
    load    v2, 5
    load    i, l_R
    draw    v1, v2, 5
    load    v1, 42
    load    v2, 5
    load    i, l_O
    draw    v1, v2, 5
    load    v1, 48
    load    v2, 5
    load    i, l_M
    draw    v1, v2, 5
BORO:
    load    v1, 5
    load    v2, 15
    load    i, l_B
    draw    v1, v2, 5
    load    v1, 15
    load    v2, 15
    load    i, l_O
    draw    v1, v2, 5
    load    v1, 25
    load    v2, 15
    load    i, l_R
    draw    v1, v2, 5
    load    v1, 35
    load    v2, 15
    load    i, l_O
    draw    v1, v2, 5
SMILE:
    load    v1, 45
    load    v2, 15
    load    i, l_SMILE
    draw    v1, v2, 5
loop:
    jmp     loop
l_SMILE:
    .pic    "    *   ",
            "  *  *  ",
            "     *  ",
            "  *  *  ",
            "    *   "
l_R:
    .pic    "   ***  ",
            "   * *  ",
            "   **** ",
            "   *  * ",
            "   *  * "
l_B:
    .pic    "  ****  ",
            "  *  *  ",
            "  ***   ",
            "  *  *  ",
            "  ****  "
l_M:
    .pic    "  ** ** ",
            "  * * * ",
            "  * * * ",
            "  *   * ",
            "  *   * "
l_F:
    .pic    "   ***  ",
            "   *    ",
            "   ***  ",
            "   *    ",
            "   *    "
l_H:
    .pic    "   * *  ",
            "   * *  ",
            "   ***  ",
            "   * *  ",
            "   * *  "
l_A:
    .pic    "   ***  ",
            "   * *  ",
            "   ***  ",
            "   * *  ",
            "   * *  "
l_L:
    .pic    "   *    ",
            "   *    ",
            "   *    ",
            "   *    ",
            "   ***  "
l_O:
    .pic    "   ***  ",
            "   * *  ",
            "   * *  ",
            "   * *  ",
            "   ***  "
