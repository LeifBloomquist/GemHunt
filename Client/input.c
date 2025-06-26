#include <cbm.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>
#include "utils.h"
#include "screen.h"
#include "input.h"
#include "network.h"

static byte send_buffer[SEND_BUFFER_SIZE];

int read_keyboard()
{
    byte c = 0;

    if (kbhit())
    {
        c = cgetc();
    }

    if (c == 0) return 0;

    switch (c)
    {
        case 'q': return 1;
        case 'w': return 2;
        case 'e': return 3;
        case 'a': return 4;
        case 'd': return 5;
        case 'z': return 6;
        case 'x': return 7;
        case 'c': return 8;
        case ' ': return 16;
        default: return 0;
    }
}

// Read the keyboard+joystick and send actions
void read_input()
{
    int bytes = 0;
    byte m = 0;

    POKE(NETWORK_CHAR, CHAR_STATE1);  // Show state

    m = read_keyboard();

    if (m == 0)
    {
        m = read_joystick();
    }

    if (m == 0)
    {
        return;
    }

    if (m != 0)
    {
        send_buffer[0] = PACKET_MOVE;
        send_buffer[1] = (m & 0x0F);
        send_buffer[2] = (m & 0x10);

        POKE(NETWORK_CHAR, CHAR_STATE2);  // Show state

        bytes = cbm_write(LFN, send_buffer, 3);

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