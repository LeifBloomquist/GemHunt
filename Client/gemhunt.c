#include <cbm.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include "utils.h"

// Network constants and defines

#define LFN 2     // The logical file number to use for I/O
#define DEV 16    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel
const char addr[] = "tcp://192.168.7.113:6420";
#define BUFFER_SIZE 100
static unsigned char buffer[BUFFER_SIZE];

// Screen constants and defines  TODO set for C64
#define WINDOW_START 0x1E00
#define WINDOW_LINE2 (WINDOW_START + 22 )
#define WINDOW_LINE3 (WINDOW_START + 44 )
#define WINDOW_LINE4 (WINDOW_START + 66 )
#define WINDOW_LINE5 (WINDOW_START + 110)
#define WINDOW_LINE6 (WINDOW_START + 132)
#define WINDOW_LINE7 (WINDOW_START + 154)
#define WINDOW_LINE8 (WINDOW_START + 176)
#define WINDOW_LINE9 (WINDOW_START + 198)

// Other game parameters
unsigned int score;
//Health


// Read from the network.   
int read_network()
{
    int bytes;

    bytes = cbm_read(LFN, buffer, BUFFER_SIZE);

    if (bytes == -1) // error
    {
        ; // TODO
    }

    // Copy Screen.  No loop for speed
    memcpy((void*)WINDOW_START, (const void *)buffer[1], 9);
    memcpy((void*)WINDOW_LINE2, (const void *)buffer[10], 9);
    memcpy((void*)WINDOW_LINE3, (const void *)buffer[19], 9);
    memcpy((void*)WINDOW_LINE4, (const void *)buffer[28], 9);
    memcpy((void*)WINDOW_LINE5, (const void *)buffer[37], 9);
    memcpy((void*)WINDOW_LINE6, (const void *)buffer[46], 9);
    memcpy((void*)WINDOW_LINE7, (const void *)buffer[55], 9);
    memcpy((void*)WINDOW_LINE8, (const void *)buffer[64], 9);
    memcpy((void*)WINDOW_LINE9, (const void *)buffer[73], 9);
    
    // Extract game stats
    score = buffer[83] + (buffer[84] << 8);

    // Display game stats

    return 0;
}



int main(void)
{
    // 0. Variables
    int result = 0;

    // 1. Set up Screen
    clrscr();
    printxy(0,0,"\x8e\x08"); // font switch to gfx/upper and disable switching

#if defined(__VIC20__)
    VIC.bg_border_color = 0x7E;
	//printf("Running on a VIC.\n");
#endif

#if defined(__C64__)
    //printf("Running on a C64.\n");
#endif

    // 2. Connect to Server
    printxy(0, 0, "connecting");
    result = cbm_open(LFN, DEV, SAN, addr);

    if (result != 0)
    {
        printxy(0, 1, "error!");
        return result;
    }

    // 3. Game Loop
    while (true)
    {
        //read_input();
        read_network();
    }

	return 0;
}