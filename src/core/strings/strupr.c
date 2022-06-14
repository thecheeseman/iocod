#include <ctype.h>
#include "iocod.h"

IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strupr(char *s1)
{
    #if !defined IC_GNUC_VERSION
    if (s1 == NULL)
        return NULL;
    #endif

    char *s = s1;
    while (*s) {
        *s = toupper(*s);
        s++;
    }
    return s1;
}
