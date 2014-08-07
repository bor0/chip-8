#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdlib.h>
#include "cpu.h"

const char keymap[] = "1234qwerasdfzxcv";

void keyboardUp(unsigned char, struct cpu *);
void keyboardDown(unsigned char, struct cpu *);
#endif
