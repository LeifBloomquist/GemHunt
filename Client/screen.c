#include <cbm.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>
#include "utils.h"
#include "screen.h"

// Prepare the game screen
void prepare_screen()
{
    int i=0;

    clrscr();

#if defined(__VIC20__)
    VIC.bg_border_color = 0x1E;    
#endif

#if defined(__C64__)   // Mimic VIC20 colours
    VIC.bgcolor0 = COLOR_WHITE;
    VIC.bordercolor = COLOR_BLUE;
#endif

    // Default character colours
    memset(COLOR_RAM, COLOR_BLUE, SCREEN_WIDTH * SCREEN_HEIGHT);

    // Network Activity
    POKE(NETWORK_CHAR_COLOUR, COLOR_RED);

    // Window
    memset((void *)(WINDOW_LINE1 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE2 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE3 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE4 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE5 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE6 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE7 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE8 + COLOR_OFFSET), COLOR_BLACK, 9);
    memset((void *)(WINDOW_LINE9 + COLOR_OFFSET), COLOR_BLACK, 9);

    // Player char color
    POKE(WINDOW_LINE5 + 4 + COLOR_OFFSET, COLOR_RED);

    // Draw the Window Border
    for (i=0; i < 9; i++)
    {
        POKE(WINDOW_LINE1   -  SCREEN_WIDTH + i,  CHAR_BORDER);  // Top
        POKE(WINDOW_LINE9   +  SCREEN_WIDTH + i,  CHAR_BORDER);  // Bottom
        POKE(WINDOW_LINE1-1 + (SCREEN_WIDTH * i), CHAR_BORDER);  // Left
        POKE(WINDOW_LINE1+9 + (SCREEN_WIDTH * i), CHAR_BORDER);  // Right
    }

    // Corners
    POKE(WINDOW_LINE1 - SCREEN_WIDTH - 1, CHAR_DIAGSE);
    POKE(WINDOW_LINE9 + SCREEN_WIDTH - 1, CHAR_DIAGNE);
    POKE(WINDOW_LINE9 + SCREEN_WIDTH + 9, CHAR_DIAGSE);
    POKE(WINDOW_LINE1 - SCREEN_WIDTH + 9, CHAR_DIAGNE);

    // Text
    printxy(1, 1,  "gem hunt multiplayer");
    printxy(5, 3,  "players:---");
    printxy(5, 18, "gems:   ---");
    printxy(5, 20, "health: ---");
}