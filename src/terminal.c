#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "common.h"

/* ─────────────────────────────────────────────────────────────────────────────
 * ANSI Escape Sequences
 * ───────────────────────────────────────────────────────────────────────────── */

#define ESC                 "\x1b"
#define CSI                 ESC "["

#define CURSOR_HOME         (CSI "H")           // move cursor to (1,1) which is the top-left corner
#define CURSOR_HIDE         (CSI "?25l")        // hide cursor
#define CURSOR_SHOW         (CSI "?25h")        // show cursor
#define CURSOR_LOCATION     (CSI "%lu;%luH")    // move cursor to (row, column)

#define TERMINAL_CLR        (CSI "2J")          // clear the entire terminal screen

/* ─────────────────────────────────────────────────────────────────────────────
 * Module State
 * ───────────────────────────────────────────────────────────────────────────── */

static struct {
    copied struct termios original;         /* Original terminal attributes */
    copied bool raw;                        /* True if raw mode is active */
} _terminal_state = {
    .original = { 0 },
    .raw      = false,
};

/* ─────────────────────────────────────────────────────────────────────────────
 * Forward Declarations
 * ───────────────────────────────────────────────────────────────────────────── */

static void terminal_setup_raw_mode_signals_();
static void terminal_sig_default_handler_(int sig);

/* ─────────────────────────────────────────────────────────────────────────────
 * Lifecycle
 * ───────────────────────────────────────────────────────────────────────────── */

copied result_t terminal_init()
{
    if (_terminal_state.raw)
    {
        return RESULT_OK(1);
    }

    /* must be a terminal */
    if (0 == isatty(STDIN_FILENO))
    {
        return RESULT_ERR(1);
    }

    /* save original terminal attributes */
    if (-1 == tcgetattr(STDIN_FILENO, &_terminal_state.original))
    {
        return RESULT_ERR(2);
    }

    terminal_enter_raw_mode();
    terminal_setup_raw_mode_signals_();
    terminal_size_query_();
    tui_use_alternate_buffer();

    atexit(terminal_quit);

    return RESULT_OK(0);
}

void terminal_quit()
{
    if (!_terminal_state.raw)
    {
        return;
    }
    terminal_leave_raw_mode();
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Raw Mode
 * ───────────────────────────────────────────────────────────────────────────── */

void terminal_enter_raw_mode()
{
    if (_terminal_state.raw)
    {
        return;
    }

    struct termios term = clone(_terminal_state.original);

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

/* ─────────────────────────────────────────────────────────────────────────────
 * Signal Handling
 * ───────────────────────────────────────────────────────────────────────────── */

static void terminal_setup_raw_mode_signals_()
{
    struct sigaction sa = { 0 };

    sa.sa_handler = terminal_sig_default_handler_;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NIL);
    sigaction(SIGTERM, &sa, NIL);
}

static void terminal_sig_default_handler_(int sig)
{
    (void) sig;
    terminal_quit();
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
    free_smart(fmt);
}

void terminal_clear()
{
    terminal_write(TERMINAL_CLR, sizeof(TERMINAL_CLR) - 1);
}

void terminal_cursor_home()
{
    terminal_write(CURSOR_HOME, sizeof(CURSOR_HOME) - 1);
}

void terminal_cursor_move(copied uint64_t row, copied uint64_t column)
{
    char coord[25] = { 0 };
    int n = snprintf(coord, sizeof(coord), CURSOR_LOCATION, row, column);
    terminal_write(coord, n);
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

copied int32_t terminal_read_raw_byte()
{
    unsigned char c;
    return (1 == read(STDIN_FILENO, &c, 1)) ? c : -1;
}

