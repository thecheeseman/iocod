#include "con_local.h"

#define CSI_START   "\033["
#define CSI_END     "\033[0m"

static const char *forecolor[] = {
    [COLOR_BLACK] =     "30;",
    [COLOR_RED] =       "31;",
    [COLOR_GREEN] =     "32;",
    [COLOR_YELLOW] =    "33;",
    [COLOR_BLUE] =      "34;",
    [COLOR_CYAN] =      "36;",
    [COLOR_MAGENTA] =   "35;",
    [COLOR_WHITE] =     "37;"
};

static const char *bgcolor[] = {
    [COLOR_BLACK] =     "40;",
    [COLOR_RED] =       "41;",
    [COLOR_GREEN] =     "42;",
    [COLOR_YELLOW] =    "43;",
    [COLOR_BLUE] =      "44;",
    [COLOR_CYAN] =      "46;",
    [COLOR_MAGENTA] =   "45;",
    [COLOR_WHITE] =     "47;"
};

void con_set_color(enum q3color fore, enum q3color back)
{
    if (fore > COLOR_WHITE || fore < COLOR_BLACK)
        fore = COLOR_WHITE;

    if (back > COLOR_WHITE || back < COLOR_BLACK)
        back = COLOR_BLACK;

    console.textcolor = fore;
    console.bgcolor = back;

    fprintf(stderr, "%s%s%s22m", CSI_START, forecolor[fore], bgcolor[back]);
}

void con_set_text_color(enum q3color fore)
{
    if (fore > COLOR_WHITE || fore < COLOR_BLACK)
        fore = COLOR_WHITE;

    console.textcolor = fore;

    fprintf(stderr, "%s%s%s22m", CSI_START, forecolor[fore], 
            bgcolor[console.bgcolor]);
}

void con_set_back_color(enum q3color back)
{
    if (back > COLOR_WHITE || back < COLOR_BLACK)
        back = COLOR_BLACK;

    console.bgcolor = back;

    fprintf(stderr, "%s%s%s22m", CSI_START, forecolor[console.textcolor],
            bgcolor[back]);
}
