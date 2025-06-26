// Misc Utility Functions

#ifndef UTILS_H
#define UTILS_H

#include <peekpoke.h>

// Data Types
typedef unsigned char byte;

// Prototypes
void printxy(unsigned char x, unsigned char y, const char* s);
byte read_joystick();
void error();
void warning();

#endif /* UTILS_H */
