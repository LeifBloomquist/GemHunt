#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include "network.h"
#include "utils.h"
#include "screen.h"

const char addr[] = "tcp://192.168.7.110:6420";
static byte buffer[BUFFER_SIZE];

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

// Read from the network and update screen
void read_network()
{
    int bytes = 0;

    POKE(NETWORK_CHAR, CHAR_STATE3);  // Show state

    POKE(0x900F, 28);  // purple
    bytes = cbm_read(LFN, buffer, BUFFER_SIZE);
    POKE(0x900F, 30);  // blue

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
    memcpy((void*)WINDOW_LINE1, (const void*)(buffer + 01), 9);
    memcpy((void*)WINDOW_LINE2, (const void*)(buffer + 10), 9);
    memcpy((void*)WINDOW_LINE3, (const void*)(buffer + 19), 9);
    memcpy((void*)WINDOW_LINE4, (const void*)(buffer + 28), 9);
    memcpy((void*)WINDOW_LINE5, (const void*)(buffer + 37), 9);
    memcpy((void*)WINDOW_LINE6, (const void*)(buffer + 46), 9);
    memcpy((void*)WINDOW_LINE7, (const void*)(buffer + 55), 9);
    memcpy((void*)WINDOW_LINE8, (const void*)(buffer + 64), 9);
    memcpy((void*)WINDOW_LINE9, (const void*)(buffer + 73), 9);

    // Extract game stats
    //score = buffer[83] + (buffer[84] << 8);

    // Display game stats

    return;
}