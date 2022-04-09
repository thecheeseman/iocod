#include <ctype.h>
#include "strings/stringlib.h"

#if !defined HAVE_STRUPR
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
#endif /* HAVE_STRUPR */
