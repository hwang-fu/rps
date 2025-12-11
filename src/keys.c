#include "keys.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include "terminal.h"

#define ESC                 (0x1b)
#define ESC_TIMEOUT_DS      (1) /* deciseconds (100ms) */

static bool _timeout_enabled = false;

static copied key_t keyboard_key_event_esc();
static copied key_t keyboard_key_event_ctrl(copied const key_t key);

static copied key_t keyboard_key_event_esc()
{
    keyboard_event_timeout_enable();
}

static copied key_t keyboard_key_event_ctrl(copied const key_t key)
{
    if (key == key_tab || key == key_enter)
    {
        return key;
    }
    return (ctrl_mask | (key - 1 + 'a'));
}

copied key_t keyboard_key_event()
{
    int32_t b = terminal_raw_byte_read();
    if (b == -1)
    {
        return key_none;
    }

    if (b == ESC)
    {
        return keyboard_key_event_esc();
    }

    if (0x01 <= b && b <= 0x1a)
    {
        return keyboard_key_event_ctrl(b);
    }

    return cast(b, key_t);
}

borrowed const char * keyboard_key_event_name_map(copied const key_t key)
{
    static copied char keyname[64] = { 0 };

    copied char meta[7] = { 0 };
    if (key & ctrl_mask)
    {
        strcat(meta, "C-");
    }
    if (key & alt_mask)
    {
        strcat(meta, "M-");
    }
    if (key & shift_mask)
    {
        strcat(meta, "S-");
    }

    copied const char * name = nil;
    copied key_t base = baseof(key);
    switch (base)
    {
        case key_unknown:       name = "<NONE>";        break;
        case key_none:          name = "<UNKNOWN>";     break;

        case key_tab:           name = "<TAB>";         break;
        case key_enter:         name = "<ENTER>";       break;
        case key_backspace:     name = "<BS>";          break;
        case key_esc:           name = "<ESC>";         break;
        case key_up:            name = "<UP>";          break;
        case key_down:          name = "<DOWN>";        break;
        case key_left:          name = "<LEFT>";        break;
        case key_right:         name = "<RIGHT>";       break;
        case key_home:          name = "<HOME>";        break;
        case key_end:           name = "<END>";         break;
        case key_page_up:       name = "<PAGE-UP>";     break;
        case key_page_down:     name = "<PAGE-DOWN>";   break;
        case key_insert:        name = "<INS>";         break;
        case key_delete:        name = "<DEL>";         break;

        case key_f1:            name = "<F1>";          break;
        case key_f2:            name = "<F2>";          break;
        case key_f3:            name = "<F3>";          break;
        case key_f4:            name = "<F4>";          break;
        case key_f5:            name = "<F5>";          break;
        case key_f6:            name = "<F6>";          break;
        case key_f7:            name = "<F7>";          break;
        case key_f8:            name = "<F8>";          break;
        case key_f9:            name = "<F9>";          break;
        case key_f10:           name = "<F10>";         break;
        case key_f11:           name = "<F11>";         break;
        case key_f12:           name = "<F12>";         break;
    }

    if (name)
    {
        snprintf(keyname, sizeof(keyname), "%s%s", meta, name);
    }
    else if (0x20 <= base && base < 0x7f)
    {
        /* printable ASCII */
        snprintf(keyname, sizeof(keyname), "%s'%c'", meta, (char) base);
    }
    else
    {
        /* non-printable */
        snprintf(keyname, sizeof(keyname), "%s0x%02X", meta, base);
    }

    return keyname;
}

void keyboard_event_timeout_enable()
{
    if (_timeout_enabled)
    {
        return;
    }

    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_cc[VMIN]  = 0;
    term.c_cc[VTIME] = ESC_TIMEOUT_DS;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    _timeout_enabled = true;
}

void keyboard_event_timeout_disable()
{
    if (!_timeout_enabled)
    {
        return;
    }

    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_cc[VMIN]  = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    _timeout_enabled = false;
}

void keyboard_event_timeout_toggle()
{
    if (_timeout_enabled)
    {
        keyboard_event_timeout_disable();
    }
    else
    {
        keyboard_event_timeout_enable();
    }
}
