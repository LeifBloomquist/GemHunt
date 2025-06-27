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

    // Debug Hack - reconnect to server
    if (c == CH_F7)
    {
        cbm_close(LFN);
        cbm_open(LFN, DEV, SAN, SERVER_ADDRESS);
        return 0;
    }

    switch (c)
    {
        case 'q': return 1;
        case 'w': return 2;
        case 'e': return 3;
        case 'a': return 4;
        case 's': return 7;
        case 'd': return 5;
        case 'z': return 6;
        case 'x': return 7;
        case 'c': return 8;
        case ' ': return 16;
        default:  return 0;
    }
}

// Read the keyboard+joystick and send actions
void read_input()
{
    int bytes = 0;
    byte m = 0;

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
        send_buffer[1] = m;
        send_buffer[2] = m;

        bytes = cbm_write(LFN, send_buffer, SEND_BUFFER_SIZE);

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