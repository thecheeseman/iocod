#include <ctype.h>
#include "../iocod.h"

#if !defined HAVE_STRUPR
IC_PUBLIC
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
