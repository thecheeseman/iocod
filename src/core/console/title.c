#include "con_local.h"

IC_PUBLIC
void con_set_title(const char *title)
{
    #ifdef IC_PLATFORM_WINDOWS
    SetConsoleTitle(title);
    #else
    // xterm allows for OSC escape sequences,
    // so if we're a compatible xterm, let's do it
    // otherwise don't bother
    if (console.term == NULL || strstr(console.term, "xterm") == NULL)
        return;

    // just assume if we don't support color we don't also
    // don't have this feature
    if (!console.ansi_color)
        return;

    fprintf(stderr, "\033]2;%s\007", title);
    #endif
}
