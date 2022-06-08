#include "con_local.h"

IC_PUBLIC
void con_set_title(const char *title)
{
    #ifdef IC_PLATFORM_WINDOWS
    SetConsoleTitle(title);
    #else
    const char *term = getenv("TERM");

    // xterm allows for OSC escape sequences,
    // so if we're a compatible xterm, let's do it
    // otherwise don't bother
    if (term == NULL || strstr(term, "xterm") == NULL)
        return;

    fprintf(stderr, "\033]2;%s\007", title);
    #endif
}
