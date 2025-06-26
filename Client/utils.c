#include <cbm.h>
#include <conio.h>
#include "utils.h"

void printxy(unsigned char x, unsigned char y, const char* s)
{
    gotoxy(x, y);

    while (*s)
        cbm_k_bsout(*s++);
}

byte read_joystick()

#if defined(__VIC20__)
{
    byte s;
    byte t;
    byte j = 0;

    s = PEEK(0x911F);    // 37151
    POKE(0x9122, 0x7F);  // 37154, 127 to switch VIA to input
    t = PEEK(0x9120);    // 37152, joystick right
    POKE(0x9122, 0xFF);  // 37154, 255 to re-enable keyboard scan

    // TODO

    return j;
}
#endif

#if defined(__C64__)
{
    byte j = PEEK(0xDC00);
    return 127-j;
}
#endif

