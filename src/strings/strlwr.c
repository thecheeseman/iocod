#include <ctype.h>
#include "strings/stringlib.h"

char *strlwr(char *s1)
{
    char *s;

    s = s1;
    while (*s) {
        *s = tolower(*s);
        s++;
    }
    return s1;
}
