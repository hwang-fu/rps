#pragma once

#define terminal_write(sequence, len) write(STDOUT_FILENO, sequence, len)

void terminal_enter_raw_mode();
void terminal_leave_raw_mode();
void terminal_toggle_raw_mode();
