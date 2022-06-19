#include "con_local.h"

bool color_string(const char *p)
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
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE,   // console doesn't support these colors
    FOREGROUND_RED                      // so we just reuse the same ones
};

DWORD color_to_attrib(enum q3color color)
{
    DWORD attrib = colors[color];

    // use console's background color
    attrib |= console.bg_attributes;

    return attrib;
}
#endif

#define CSI_START   "\033["
#define CSI_END     "\033[0m"

static char *forecolor[] = {
    [COLOR_BLACK] =     "30",
    [COLOR_RED] =       "31",
    [COLOR_GREEN] =     "32",
    [COLOR_YELLOW] =    "33",
    [COLOR_BLUE] =      "34",
    [COLOR_CYAN] =      "36",
    [COLOR_MAGENTA] =   "35",
    [COLOR_WHITE] =     "37",
    [COLOR_PURPLE] =    "94",   // these aren't guarenteed
    [COLOR_ROSE] =      "91"    // but most modern xterms should have different
                                // colors here
};

static char *bgcolor[] = {
    [COLOR_BLACK] =     "40",
    [COLOR_RED] =       "41",
    [COLOR_GREEN] =     "42",
    [COLOR_YELLOW] =    "43",
    [COLOR_BLUE] =      "44",
    [COLOR_CYAN] =      "46",
    [COLOR_MAGENTA] =   "45",
    [COLOR_WHITE] =     "47"
};

char *color_to_ascii_code(enum q3color color)
{
    return forecolor[color];
}
