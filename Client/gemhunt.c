#include <cbm.h>
#include <conio.h>
#include "utils.h"

int main(void)
{
    clrscr();
    printxy(0,0,"\x8e"); // font switch to gfx/upper

    printxy(0,0,"Hello World");	

#if defined(__VIC20__)
    VIC.bg_border_color = 0x7E;
	//printf("Running on a VIC.\n");
#endif

#if defined(__C64__)
    //printf("Running on a C64.\n");
#endif

	return 0;
}