#pragma once

#include "common.h"

#define ctrl_mask           (0x1000)
#define alt_mask            (0x2000)
#define shift_mask          (0x4000)
#define mask                (ctrl_mask | alt_mask | shift_mask)
#define baseof(key)         ((key) & (~mask))

typedef int32_t key_t;

enum
{
    key_unknown   = -2,
    key_none      = -1,

    key_tab       = 9,      /* ctrl + i */
    key_enter     = 13,     /* ctrl + m */
    key_backspace = 127,
    key_esc       = 256,

    key_up,
    key_down,
    key_left,
    key_right,
    key_home,
    key_end,
    key_page_up,
    key_page_down,

    key_insert,
    key_delete,

    key_f1,
    key_f2,
    key_f3,
    key_f4,
    key_f5,
    key_f6,
    key_f7,
    key_f8,
    key_f9,
    key_f10,
    key_f11,
    key_f12,
};

copied key_t keyboard_key_event();
borrowed const char * keyboard_key_event_name_map(copied const key_t key);

void keyboard_event_timeout_enable();
void keyboard_event_timeout_disable();
void keyboard_event_timeout_toggle();
