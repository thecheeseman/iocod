#include <ctype.h>
#include "strings/stringlib.h"

char *strupr(char *s1)
{
    char *s;

    s = s1;
    while (*s) {
        *s = toupper(*s);
        s++;
    }
    return s1;
}
