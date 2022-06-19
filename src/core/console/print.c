#include "con_local.h"

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
    static char buffer[MAX_PRINT_MSG] = { 0 };
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
                    snprintf(buffer, sizeof(buffer), "\033[%sm", 
                             color_to_ascii_code(color));
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
