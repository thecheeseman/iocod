/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include "con_local.h"
#include <signal.h>

#ifndef IC_PLATFORM_WINDOWS
#include <term.h>
#endif

struct console_data console;

IC_PUBLIC
bool con_initialized(void)
{
    return console.initialized;
}

#ifdef IC_PLATFORM_WINDOWS
static BOOL WINAPI con_sigint(DWORD sig)
{
    switch (sig) {
    case CTRL_C_EVENT:
        sys_signal_handler(SIGINT);
        return TRUE;
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        sys_signal_handler(SIGTERM);
        return TRUE;
    default:
        return FALSE;
    }
}

IC_PUBLIC
void con_init(void)
{
    memset(&console, 0, sizeof(console));

    if (!SetConsoleCtrlHandler(con_sigint, TRUE))
        sys_handle_error_exit(__func__);

    if ((console.hin = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        sys_handle_error_exit(__func__);

    if ((console.hout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        sys_handle_error_exit(__func__);

    // enable mouse wheel scrolling
    GetConsoleMode(console.hin, &console.original_mode);
    SetConsoleMode(console.hin, console.original_mode & ~ENABLE_MOUSE_INPUT);

    DWORD mode;
    GetConsoleMode(console.hout, &mode);

    // check if we can do virtual terminal processing
    mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (SetConsoleMode(console.hout, mode)) {
        if (config_initialized() && config_console_colors())
            console.ansi_color = true; // we can support it
    }

    FlushConsoleInputBuffer(console.hin);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(console.hout, &info);
    console.attributes = info.wAttributes;
    console.bg_attributes = console.attributes &
        (BACKGROUND_BLUE | BACKGROUND_GREEN |
         BACKGROUND_RED | BACKGROUND_INTENSITY);

    // init history tbd

    SetConsoleTextAttribute(console.hout, console.attributes);
    SetConsoleTextAttribute(console.hout,
                            color_to_attrib(COLOR_WHITE));

    console.initialized = true;
}

IC_PUBLIC
void con_shutdown(void)
{
    con_hide();

    SetConsoleMode(console.hin, console.original_mode);
    SetConsoleCursorInfo(console.hout, &console.original_cursorinfo);
    SetConsoleTextAttribute(console.hout, console.attributes);

    CloseHandle(console.hout);
    CloseHandle(console.hin);
}
#else
static void con_sigcont(int signum)
{
    UNUSED_PARAM(signum);

    con_init();
}

IC_PUBLIC
bool stdin_is_atty(void)
{
    bool atty = isatty(STDIN_FILENO);
    bool dumbterm = false;

    if (console.term == NULL || strcmp(console.term, "raw") == 0 ||
        strcmp(console.term, "dumb") == 0)
        dumbterm = true;

    return atty && !dumbterm;
}

IC_PUBLIC
void con_init(void)
{
    memset(&console, 0, sizeof(console));

    // setupterm will auto exit if there's an error
    console.term = getenv("TERM");
    setupterm(console.term, STDOUT_FILENO, (int *) 0);

    // not foolproof, but an easy check
    if (config_initialized() && config_console_colors()) {
        if (tigetnum("colors") > 0)
            console.ansi_color = true;
    }

    console.num_lines = tigetnum("lines");
    console.num_columns = tigetnum("cols");

    // ignore tty in/tty out signals
    // if the process is running non-interactively these can turn into
    // a SIGSTP
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    // if SIGCONT is received, reinit console
    signal(SIGCONT, con_sigcont);

    // make stdin non-blocking
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    if (!stdin_is_atty()) {
        fprintf(stderr, _("tty console mode disabled\n"));

        console.stdin_active = true;
        return;
    }

    memset(&console.field, 0, sizeof(console.field));

    tcgetattr(STDIN_FILENO, &console.tc);
    console.tty_erase = console.tc.c_cc[VERASE];
    console.tty_eof = console.tc.c_cc[VEOF];

    struct termios tc = console.tc;

    /*
        ECHO: don't echo input characters
        ICANON: enable canonical mode.  This  enables  the  special
        characters  EOF,  EOL,  EOL2, ERASE, KILL, REPRINT,
        STATUS, and WERASE, and buffers by lines.
        ISIG: when any of the characters  INTR,  QUIT,  SUSP,  or
        DSUSP are received, generate the corresponding signal
    */
    tc.c_lflag &= ~(ECHO | ICANON);

    /*
        ISTRIP strip off bit 8
        INPCK enable input parity checking
    */
    tc.c_iflag &= ~(ISTRIP | INPCK);

    tc.c_cc[VMIN] = 1;
    tc.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSADRAIN, &tc);

    console.on = true;
    console.hide = 1;

    con_show();

    console.initialized = true;
}

IC_PUBLIC
void con_shutdown(void)
{
    if (console.on) {
        con_hide();
        tcsetattr(STDIN_FILENO, TCSADRAIN, &console.tc);
    }

    // restore blocking to stdin reads
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) & ~O_NONBLOCK);

    del_curterm(cur_term);
}
#endif
