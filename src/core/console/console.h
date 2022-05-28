#ifndef IC_CORE_CONSOLE_H
#define IC_CORE_CONSOLE_H

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#else
#include <termios.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#endif

#define MAX_HISTORY 32

/*
 * internal console use only
 */

#define MAX_EDIT_LINE 256
struct field {
    int cursor;
    int scroll;
    int width;
    char buffer[MAX_EDIT_LINE];
};

/**
 * @brief Console data
*/
struct console_data {
    struct cvar *ttycon;
    struct cvar *tty_colors;

    #ifdef IC_PLATFORM_WINDOWS
    /* windows has the GUI console */
    HWND window;
    HWND window_buffer;

    HWND button_clear;
    HWND button_copy;
    HWND button_quit;

    HWND error_box;
    HWND error_text;

    HBITMAP logo;
    HBITMAP clear_bitmap;

    HBRUSH edit_background;
    HBRUSH error_background;

    HFONT buffer_font;
    HFONT button_font;

    HWND input_line;

    char error_string[80];
    char console_text[512];
    char returned_text[512];

    int vis_level;

    bool quit_on_close;

    int width;
    int height;

    WNDPROC input_line_handler;
    #else
    /* linux / macos just use standard termios */
    struct termios tty_tc;
    struct field edit_lines[MAX_HISTORY];

    bool stdin_active;
    bool ttycon_enabled;
    int ttycon_hide;
    int tty_erase;
    int tty_eof;

    int history_current;
    int history_count;
    #endif
};

extern struct console_data console;

extern struct cvar *com_viewlog;
extern struct cvar *com_dedicated;

/*
 * con_create.c
 */
void con_create(void);
void con_destroy(void);

/*
 * con_input.c
 */
#ifdef IC_PLATFORM_WINDOWS
void con_input_handler(HWND window, UINT message, WPARAM param1, LPARAM param2);
#endif

void con_input_init(void);
void con_input_shutdown(void);

/*
 * con_print.c
 */
void con_print(char *msg);

#endif /* IC_CORE_CONSOLE_H */
