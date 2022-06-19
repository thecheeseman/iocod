#ifndef CON_LOCAL_H
#define CON_LOCAL_H

#include "iocod.h"

#include <stdlib.h>

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#else
#include <termios.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#endif

#define TTY_PROMPT "] "

#define MAX_EDIT_LINE 256

/**
 * @brief Console field structure, for history.
*/
struct field {
    size_t cursor;
    int scroll;
    int width;

    char buffer[MAX_EDIT_LINE];
};

#define MAX_HISTORY 32
extern struct field hist_field[MAX_HISTORY];
extern int hist_current;
extern int hist_count;

void history_add(struct field *field);
struct field *history_previous(void);
struct field *history_next(void);

/**
 * @brief Local console data.
*/
struct console_data {
    enum q3color bgcolor;
    enum q3color textcolor;

    /**
     * @brief Flag if the tty console is currently on.
    */
    bool on;

    /**
     * @brief ANSI color support.
     *
     * On Windows, this is via the ENABLE_VIRTUAL_TERMINAL_PROCESSING console
     * mode.
    */
    bool ansi_color;

    int num_lines;
    int num_columns;

    #ifdef IC_PLATFORM_WINDOWS
    HANDLE hin;
    HANDLE hout;

    DWORD original_mode;
    CONSOLE_CURSOR_INFO original_cursorinfo;

    DWORD attributes;
    DWORD bg_attributes;

    char line[MAX_EDIT_LINE];
    int line_len;
    int cursor;
    #else

    const char *term;

    /**
     * @brief Flag if the console is currently in non-interactive mode.
    */
    bool stdin_active;

    /**
     * @brief Termios data
    */
    struct termios tc;

    /**
     * @brief Current console field data.
    */
    struct field field;

    /**
     * @brief TTY erase char.
    */
    int tty_erase;

    /**
     * @brief TTY EOF char.
    */
    int tty_eof;

    /**
     * @brief Number of hidden characters waiting to be shown.
    */
    int hide;

    /**
     * @brief Number of overdue characters (due to no trailing '\n').
    */
    int overdue;
    #endif
};

extern struct console_data console;

void con_back(void);
void con_hide(void);
void con_show(void);

void con_set_bg_color(enum q3color color);
void con_set_text_color(enum q3color color);

bool color_string(const char *p);
char *color_to_ascii_code(enum q3color color);

#ifdef IC_PLATFORM_WINDOWS
DWORD color_to_attrib(enum q3color color);
#endif

#endif /* CON_LOCAL_H */
