#include "con_local.h"

static bool color_string(const char *p)
{
    if (p == NULL || p[0] != '^' || p[1] == '\0')
        return false;

    if (p[1] < 0 || isalnum(p[1]) == 0)
        return false;

    return true;
}

#define MAX_PRINT_MSG 4096

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
                fputs("\033[0m\n", stderr);
                msg++;
            } else {
                int color = (int) (*(msg + 1) - '0');
                snprintf(buffer, sizeof(buffer), "\033[%dm", color + 30);
                fputs(buffer, stderr);
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

#ifdef IC_PLATFORM_WINDOWS
void con_print(const char *msg)
{
    con_hide();

    color_print(msg);

    con_show();
}
#else
IC_PUBLIC
void con_print(const char *msg)
{
    if (msg == NULL || *msg == '\0')
        return;

    con_hide();

    color_print(msg);

    if (!console.on)
        return;

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
}
#endif
