#include <cbm.h>
#include <conio.h>

void printxy(unsigned char x, unsigned char y, const char* s)
{
    gotoxy(x, y);

    while (*s)
        cbm_k_bsout(*s++);
}
