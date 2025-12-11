#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>

/* ─────────────────────────────────────────────────────────────────────────────
 * ANSI Escape Sequences
 * ───────────────────────────────────────────────────────────────────────────── */

#define ESC                 "\x1b"
#define CSI                 ESC "["

#define CURSOR_HIDE         (CSI "?25l")        // hide cursor
#define CURSOR_SHOW         (CSI "?25h")        // show cursor

/* ─────────────────────────────────────────────────────────────────────────────
 * Module State
 * ───────────────────────────────────────────────────────────────────────────── */

static struct {
    copied struct termios original;         /* Original terminal attributes */
    copied bool           raw;              /* True if raw mode is active */
} _terminal_state = {
    .original = { 0 },
    .raw      = false,
};

/* ─────────────────────────────────────────────────────────────────────────────
 * Forward Declarations
 * ───────────────────────────────────────────────────────────────────────────── */

static void terminal_setup_raw_mode_signals_();
static void terminal_sig_default_handler_(copied int sig);

/* ─────────────────────────────────────────────────────────────────────────────
 * Raw Mode
 * ───────────────────────────────────────────────────────────────────────────── */

void terminal_enter_raw_mode()
{
    if (_terminal_state.raw)
    {
        return;
    }

    /* STDIN must be a terminal */
    if (0 == isatty(STDIN_FILENO))
    {
        exit(EXIT_FAILURE);
    }

    /* save original terminal attributes */
    if (-1 == tcgetattr(STDIN_FILENO, &_terminal_state.original))
    {
        exit(EXIT_FAILURE);
    }

    copied struct termios term = clone(_terminal_state.original);

    /*
     * Input flags:
     * - IXON: Disable Ctrl-S/Ctrl-Q flow control
     * - ICRNL: Disable CR to NL translation
     * - BRKINT: Disable break condition signals
     * - INPCK: Disable parity checking
     * - ISTRIP: Disable stripping of 8th bit
     */
    term.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);

    /*
     * Output flags:
     * - OPOST: Disable output processing
     */
    term.c_oflag &= ~(OPOST);

    /*
     * Control flags:
     * - CS8: Set character size to 8 bits
     */
    term.c_cflag |= (CS8);

    /*
     * Local flags:
     * - ECHO: Disable echo
     * - ICANON: Disable canonical mode (line buffering)
     * - ISIG: Disable Ctrl-C/Ctrl-Z signals
     * - IEXTEN: Disable Ctrl-V
     */
    term.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    /*
     * Control characters:
     * - VMIN: Minimum bytes for read (1 = blocking read)
     * - VTIME: Timeout (0 = no timeout)
     */
    term.c_cc[VMIN]  = 1;
    term.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);

    terminal_setup_raw_mode_signals_();

    atexit(terminal_leave_raw_mode);

    _terminal_state.raw = true;
}

void terminal_leave_raw_mode()
{
    if (!_terminal_state.raw)
    {
        return;
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_terminal_state.original);

    _terminal_state.raw = false;
}

void terminal_toggle_raw_mode()
{
    if (_terminal_state.raw)
    {
        terminal_leave_raw_mode();
    }
    else
    {
        terminal_enter_raw_mode();
    }
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Signal Handling
 * ───────────────────────────────────────────────────────────────────────────── */

static void terminal_setup_raw_mode_signals_()
{
    copied struct sigaction sa = { 0 };

    sa.sa_handler = terminal_sig_default_handler_;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, nil);
    sigaction(SIGTERM, &sa, nil);
}

static void terminal_sig_default_handler_(copied int sig)
{
    (void) sig;
    terminal_leave_raw_mode();
    _exit(128 + sig);
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Terminal Screen (TUI) Operations
 * ───────────────────────────────────────────────────────────────────────────── */
void terminal_writef(borrowed const char * fmt, ...)
{
    if (!fmt)
    {
        return;
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fflush(stdout);
    va_end(args);
}

void terminal_writef_owned(owned char * fmt, ...)
{
    if (!fmt)
    {
        return;
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fflush(stdout);
    va_end(args);
    free(fmt);
}

void terminal_cursor_hide()
{
    terminal_write(CURSOR_HIDE, sizeof(CURSOR_HIDE) - 1);
}

void terminal_cursor_show()
{
    terminal_write(CURSOR_SHOW, sizeof(CURSOR_SHOW) - 1);
}

void terminal_flush()
{
    fflush(stdout);
}

copied int32_t terminal_raw_byte_read()
{
    copied unsigned char c;
    return (1 == read(STDIN_FILENO, &c, 1)) ? c : -1;
}

