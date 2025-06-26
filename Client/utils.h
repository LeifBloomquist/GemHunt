#ifndef UTILS_H
#define UTILS_H

#include <peekpoke.h>

// Data Types
typedef unsigned char byte;

void printxy(unsigned char x, unsigned char y, const char* s);
byte read_joystick();

#endif /* UTILS_H */
