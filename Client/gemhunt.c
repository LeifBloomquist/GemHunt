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
        error();
        while (true) {}
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
            error();
            return;
        }

        if (bytes != 3)  // not all bytes sent
        {
            warning();
            return;  
        }
    }
}

// Read from the network and update screen
void read_network()
{
    int bytes=0;

    POKE(NETWORK_CHAR, CHAR_STATE3);  // Show state

    POKE((int)36879, 28);  // purple
    bytes = cbm_read(LFN, buffer, BUFFER_SIZE);
    POKE((int)36879, 30);

    if (bytes == 0) // No data
    {
        return;
    }

    if (bytes == -1) // error
    {
        error();
        return;
    }

    if (bytes != 82)  // not all bytes read
    {
        warning();        
        return;
    }

    POKE(NETWORK_CHAR, CHAR_STATE4);  // Show state

    // Copy Screen.  No loop for speed (TODO, may need to further optimize with asm)
    memcpy((void*)WINDOW_LINE1, (const void *)(buffer+01), 9);
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