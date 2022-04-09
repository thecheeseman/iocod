#include <ctype.h>
#include "strings/stringlib.h"

#if !defined HAVE_STRLWR
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
#endif /* HAVE_STRLWR */
