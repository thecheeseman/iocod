#include "game_local.h"
#include "strings/stringlib.h"

char *concat_args(int start) 
{
    int	c;
    static char	line[MAX_STRING_CHARS];
    int	len;

    len = 0;
    c = trap_argc();

    for (int i = start; i < c; i++) {
        char arg[MAX_STRING_CHARS];
        trap_argv(i, arg, sizeof(arg));

        size_t tlen = strlen(arg);
        if (len + tlen >= MAX_STRING_CHARS - 1)
            break;

        memcpy(line + len, arg, tlen);
        len += tlen;
        if (i != c - 1) {
            line[len] = ' ';
            len++;
        }
    }

    line[len] = 0;

    return line;
}
