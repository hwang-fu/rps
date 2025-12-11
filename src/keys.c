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
static copied key_t keyboard_key_event_ctrl_alt(copied const key_t key);
static copied key_t keyboard_key_event_csi();
static copied key_t keyboard_key_event_csi_ext(copied const key_t key);
static copied key_t keyboard_key_event_ss3();

static copied key_t keyboard_key_event_csi_ext(copied const key_t key)
{
    copied int32_t n = key - '0';

    copied int32_t b = terminal_raw_byte_read();
    if (b == -1)
    {
        keyboard_event_timeout_disable();
        return key_unknown;
    }

    if ('0' <= b && b <= '9')
    {
        n = (n * 10) + (b - '0');
        b = terminal_raw_byte_read();
    }

    keyboard_event_timeout_disable();
    if (b != '~')
    {
        return key_unknown;
    }

    switch (n)
    {
        case 1:     return key_home;
        case 2:     return key_insert;
        case 3:     return key_delete;
        case 4:     return key_end;
        case 5:     return key_page_up;
        case 6:     return key_page_down;

        case 11:    return key_f1;
        case 12:    return key_f2;
        case 13:    return key_f3;
        case 14:    return key_f4;
        case 15:    return key_f5;
        case 17:    return key_f6;
        case 18:    return key_f7;
        case 19:    return key_f8;
        case 20:    return key_f9;
        case 21:    return key_f10;
        case 23:    return key_f11;
        case 24:    return key_f12;

        default:    return key_unknown;
    }
}

static copied key_t keyboard_key_event_csi()
{
    copied int32_t b = terminal_raw_byte_read();
    switch (b)
    {
        case 'A':
        {
            keyboard_event_timeout_disable();
            return key_up;
        } break;

        case 'B':
        {
            keyboard_event_timeout_disable();
            return key_down;
        } break;

        case 'C':
        {
            keyboard_event_timeout_disable();
            return key_right;
        } break;

        case 'D':
        {
            keyboard_event_timeout_disable();
            return key_left;
        } break;

        case 'H':
        {
            keyboard_event_timeout_disable();
            return key_home;
        } break;

        case 'F':
        {
            keyboard_event_timeout_disable();
            return key_end;
        } break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            return keyboard_key_event_csi_ext(b);
        } break;

        default:
        {
            keyboard_event_timeout_disable();
            return key_none;
        } break;
    }
}

static copied key_t keyboard_key_event_ss3()
{
    copied int32_t key = terminal_raw_byte_read();
    keyboard_event_timeout_disable();
    switch (key)
    {
        case 'P': return key_f1     ;
        case 'Q': return key_f2     ;
        case 'R': return key_f3     ;
        case 'S': return key_f4     ;
        case 'H': return key_home   ;
        case 'F': return key_end    ;
        /* Some terminals also send these for arrows */
        case 'A': return key_up     ;
        case 'B': return key_down   ;
        case 'C': return key_right  ;
        case 'D': return key_left   ;
        default : return key_unknown;
    }
}

static copied key_t keyboard_key_event_esc()
{
    keyboard_event_timeout_enable();

    copied int32_t b = terminal_raw_byte_read();
    if (b == -1)
    {
        keyboard_event_timeout_disable();
        return key_esc;
    }

    // alt + <key>
    if (0x20 <= b && b < 0x7f && b != '[' && b != 'O')
    {
        keyboard_event_timeout_disable();
        return (alt_mask | b);
    }

    // ctrl + alt + <key>
    if (0x01 <= b && b <= 0x1a)
    {
        return keyboard_key_event_ctrl_alt(b);
    }

    // CSI
    if (b == '[')
    {
        return keyboard_key_event_csi();
    }

    // SS3
    if (b == 'O')
    {
        return keyboard_key_event_ss3();
    }

    keyboard_event_timeout_disable();
    return key_unknown;
}

static copied key_t keyboard_key_event_ctrl_alt(copied const key_t key)
{
    keyboard_event_timeout_disable();
    if (key == key_tab || key == key_enter)
    {
        return key;
    }
    return (ctrl_mask | alt_mask | (key - 1 + 'a'));
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

// NOTE: this function is NOT thread-safe, use it only in single-thread situation.
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
        case key_unknown:       name = "<UNKNOWN>";     break;
        case key_none:          name = "<NONE>";        break;

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
