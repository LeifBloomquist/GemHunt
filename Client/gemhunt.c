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
    clrscr();
    printxy(0, 0, "\x8e\x08"); // font switch to gfx/upper and disable switching
    POKE(NETWORK_CHAR_COLOUR, COLOR_RED);   

#if defined(__VIC20__)
    VIC.bg_border_color = 0x7E;
    memset(COLOR_RAM, COLOR_BLACK, 22*23);
#endif

#if defined(__C64__)
    memset(COLOR_RAM, COLOR_BLACK, 40*25);
#endif
}

// Connectg to the server
void connect_server()
{
    int result = 0;
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
    byte j = read_joystick();    

    if (j != 0)
    {
        buffer[0] = 1;
        buffer[1] = (j & 0x0F);
        buffer[2] = (j & 0x10);
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
int read_network()
{
    int bytes=0;
    //bytes = cbm_read(LFN, buffer, BUFFER_SIZE);

    if (bytes == -1) // error
    {
        ; // TODO
    }

    if (bytes != 89)  // not all bytes read
    {
        ; // TODO
    }


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

    // Animate
    asm("inc %w", NETWORK_CHAR);
    
    // Extract game stats
    score = buffer[83] + (buffer[84] << 8);

    // Display game stats

    return 0;
}

// ------------------------------------------------------------------------------

int main(void)
{
    // 0. Variables


    // 1. Set up Screen
    prepare_screen();

    // 2. Connect to Server
    connect_server();

    // 3. Game Loop
    while (true)
    {
        read_input();
        read_network();
    }

	return 0;
}