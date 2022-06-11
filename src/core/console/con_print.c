#include "con_local.h"

static bool color_string(const char *p)
{
    if (p == NULL || p[0] != '^' || p[1] == '\0')
        return false;

    if (p[1] < 0 || isalnum(p[1]) == 0)
        return false;

    return true;
}

#ifdef IC_PLATFORM_WINDOWS
static DWORD colors[] = {
    0,
    FOREGROUND_RED,
    FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_BLUE,
    FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};

static DWORD color_to_attrib(enum q3color color)
{
    DWORD attrib;

    if (color == COLOR_WHITE) {
        attrib = console.attributes;
    } else {
        attrib = colors[color];

        // use console's background color
        attrib |= console.bg_attributes;
    }

    return attrib;
}
#endif

#define MAX_PRINT_MSG 4096

/*
 * color printing is annoying. on *nix/macos we have support for VT100/ANSI
 * color coding, with the \033[xxx; format and that's easy enough. some
 * consoles don't support this, and we can sort of check with terminfo but
 * not all consoles support terminfo either...
 * 
 * windows sucks in general but at least win10+ supports VT100/ANSI codes, 
 * but none of the previous versions do... so we have to fall back on win32
 * console attributes
 */
static void color_print(const char *msg)
{
    static char buffer[MAX_PRINT_MSG];
    int length = 0;

    while (*msg != '\0') {
        console.on = (*msg == '\n');

        if (color_string(msg) || *msg == '\n') {
            if (length > 0) {
                buffer[length] = '\0';
                fputs(buffer, stderr);
                length = 0;
            }

            if (*msg == '\n') {
                if (console.ansi_color) {
                    fputs("\033[0m\n", stderr);
                } else {
                    #ifdef IC_PLATFORM_WINDOWS
                    SetConsoleTextAttribute(console.hout, 
                                            color_to_attrib(COLOR_WHITE));
                    #endif

                    fputs("\n", stderr);
                }

                msg++;
            } else {
                int color = (int) (*(msg + 1) - '0');

                if (console.ansi_color) {
                    snprintf(buffer, sizeof(buffer), "\033[%dm", color + 30);
                    fputs(buffer, stderr);
                } else {
                    #ifdef IC_PLATFORM_WINDOWS
                    SetConsoleTextAttribute(console.hout,
                                            color_to_attrib(color));
                    #endif
                }
                msg += 2;
            }
        } else {
            if (length >= MAX_PRINT_MSG - 1)
                break;

            buffer[length] = *msg;
            length++;
            msg++;
        }
    }

    if (length > 0) {
        buffer[length] = '\0';
        fputs(buffer, stderr);
    }
}

void con_print(const char *msg)
{
    if (msg == NULL || *msg == '\0')
        return;

    con_hide();

    color_print(msg);

    #ifdef IC_PLATFORM_WINDOWS
    con_show();
    #else
    if (msg[strlen(msg) - 1] == '\n') {
        con_show();

        while (console.overdue > 0) {
            con_show();
            console.overdue--;
        }
    } else {
        // defer calling con_show
        console.overdue++;
    }
    #endif
}
