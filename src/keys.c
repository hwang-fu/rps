#include "keys.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include "terminal.h"

#define ESC                 (0x1b)
#define ESC_TIMEOUT_DS      (1) /* deciseconds (100ms) */


copied key_t keyboard_key_event_read()
{
    int32_t b = terminal_raw_byte_read();
    if (b == -1)
    {
        return key_none;
    }

    if (b == ESC)
    {
    }

    if (0x01 <= b && b <= 0x1a)
    {
    }

    return cast(b, key_t);
}

