#include <ctype.h>
#include "../iocod.h"

#if !defined HAVE_STRLWR
IC_PUBLIC
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
