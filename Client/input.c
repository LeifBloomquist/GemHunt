#include <cbm.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>
#include "utils.h"
#include "screen.h"
#include "input.h"
#include "network.h"

static byte send_buffer[SEND_BUFFER_SIZE];

// Read the joystick and send actions
void read_input()
{
    int bytes = 0;
    byte j = 0;

    POKE(NETWORK_CHAR, CHAR_STATE1);  // Show state

    j = read_joystick();

    if (j != 0)
    {
        send_buffer[0] = 1;
        send_buffer[1] = (j & 0x0F);
        send_buffer[2] = (j & 0x10);

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