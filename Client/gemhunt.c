#include <cbm.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include "utils.h"
#include "screen.h"
#include "input.h"
#include "network.h"

// Other game parameters
unsigned int score;
//Health

void initialize(void)
{
    printxy(0, 0, "\x8e"); // font switch to gfx/upper
    POKE(0x0291, 0xF0);    // disable font switching
}

// ------------------------------------------------------------------------------

int main(void)
{
    // 0. Initialize
    initialize();
    
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
