#pragma once

#include "common.h"

void terminal_enter_raw_mode();
void terminal_leave_raw_mode();
void terminal_toggle_raw_mode();

#define terminal_write(sequence, len) write(STDOUT_FILENO, sequence, len)
void terminal_writef(borrowed const char * fmt, ...);
void terminal_writef_owned(owned char * fmt, ...);

void terminal_cursor_hide();
void terminal_cursor_show();
void terminal_flush();

copied int32_t terminal_raw_byte_read();
