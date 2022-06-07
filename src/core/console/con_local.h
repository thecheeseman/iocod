#ifndef CON_LOCAL_H
#define CON_LOCAL_H

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#else
#include <termios.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#define TTY_PROMPT "]"

#define MAX_EDIT_LINE 256

/**
 * @brief Console field structure, for history.
*/
struct field {
    int cursor;
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
    #ifdef IC_PLATFORM_WINDOWS

    #else
    bool on;
    bool stdin_active;

    struct termios tc;
    struct field con;

    int erase;
    int eof;
    int hide;
    int show_overdue;

    #endif
};

extern struct console_data condata;

void con_hide(void);
void con_show(void);

#endif /* CON_LOCAL_H */
