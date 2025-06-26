#include <cbm.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include "utils.h"
#include "screen.h"
#include "network.h"

// Other game parameters
unsigned int score;
//Health

// Prepare the game screen
void prepare_screen()
{
    int i=0;

    clrscr();
    printxy(0, 0, "\x8e"); // font switch to gfx/upper
    POKE(0x0291, 0xF0);    // disable font switching   

#if defined(__VIC20__)
    VIC.bg_border_color = 0x1E;    
#endif

#if defined(__C64__)   // Mimic VIC20 colours
    VIC.bgcolor0 = COLOR_WHITE;
    VIC.bordercolor = COLOR_BLUE;
#endif

    memset(COLOR_RAM, COLOR_BLACK, SCREEN_WIDTH * SCREEN_HEIGHT);
    POKE(NETWORK_CHAR_COLOUR, COLOR_RED);

    // Draw the Window Border
    for (i=0; i < 9; i++)
    {
        POKE(WINDOW_START   -  SCREEN_WIDTH + i,  CHAR_BORDER);  // Top
        POKE(WINDOW_LINE9   +  SCREEN_WIDTH + i,  CHAR_BORDER);  // Bottom
        POKE(WINDOW_START-1 + (SCREEN_WIDTH * i), CHAR_BORDER);  // Left
        POKE(WINDOW_START+9 + (SCREEN_WIDTH * i), CHAR_BORDER);  // Right
    }

    POKE(WINDOW_START - SCREEN_WIDTH - 1, CHAR_DIAGSE);
    POKE(WINDOW_LINE9 + SCREEN_WIDTH - 1, CHAR_DIAGNE);
    POKE(WINDOW_LINE9 + SCREEN_WIDTH + 9, CHAR_DIAGSE);
    POKE(WINDOW_START - SCREEN_WIDTH + 9, CHAR_DIAGNE);

    printxy(1, 1,  "gem hunt multiplayer");
    printxy(5, 3,  "players: 0");
    printxy(5, 18, "gems:   0");
    printxy(5, 20, "health: 100");
}

// Connect to the server
void connect_server()
{
    int result = 0;

    clrscr();
    printxy(0, 0, "connecting to server");

    result = cbm_open(LFN, DEV, SAN, addr);
    if (result != 0)
    {
        printxy(0, 2, "error!");
       // while (true) {}
    }
}

// Read the joystick and send actions
void read_input()
{
    int bytes = 0;
    byte j = 0;

    POKE(NETWORK_CHAR, CHAR_STATE1);  // Show state

    j = read_joystick();

    if (j != 0)
    {
        buffer[0] = 1;
        buffer[1] = (j & 0x0F);
        buffer[2] = (j & 0x10);

        POKE(NETWORK_CHAR, CHAR_STATE2);  // Show state

        bytes = cbm_write(LFN, buffer, 3);

        if (bytes == -1) // error
        {
            ; // TODO
        }

        if (bytes != 3)  // not all bytes sent
        {
            ; // TODO
        }
    }
}

// Read from the network and update screen
void read_network()
{
    int bytes=0;

    POKE(NETWORK_CHAR, CHAR_STATE3);  // Show state

    //bytes = cbm_read(LFN, buffer, BUFFER_SIZE);

    if (bytes == 0) // No data
    {
        //return;  TODO
    }

    if (bytes == -1) // error
    {
        ; // TODO
    }

    if (bytes != 89)  // not all bytes read
    {
        ; // TODO
    }

    POKE(NETWORK_CHAR, CHAR_STATE4);  // Show state

    // Copy Screen.  No loop for speed (TODO, may need to further optimize with asm)
    memcpy((void*)WINDOW_START, (const void *)(buffer+01), 9);
    memcpy((void*)WINDOW_LINE2, (const void *)(buffer+10), 9);
    memcpy((void*)WINDOW_LINE3, (const void *)(buffer+19), 9);
    memcpy((void*)WINDOW_LINE4, (const void *)(buffer+28), 9);
    memcpy((void*)WINDOW_LINE5, (const void *)(buffer+37), 9);
    memcpy((void*)WINDOW_LINE6, (const void *)(buffer+46), 9);
    memcpy((void*)WINDOW_LINE7, (const void *)(buffer+55), 9);
    memcpy((void*)WINDOW_LINE8, (const void *)(buffer+64), 9);
    memcpy((void*)WINDOW_LINE9, (const void *)(buffer+73), 9);
    
    // Extract game stats
    score = buffer[83] + (buffer[84] << 8);

    // Display game stats

    return;
}

// ------------------------------------------------------------------------------

int main(void)
{
    // 0. Variables
    
    // 1. Connect to Server
    connect_server();

    // 2. Set up Screen
    prepare_screen();

    // 3. Game Loop
    while (true)
    {
        read_input();
        read_network();
    }

	return 0;
}