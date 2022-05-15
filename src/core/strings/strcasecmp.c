#include <ctype.h>
#include "../iocod.h"

IC_PUBLIC
IC_NON_NULL(1, 2)
int _ic_strcasecmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    int result;

    if (p1 == p2)
        return 0;

    #if !defined IC_GNUC_VERSION
    if (p1 == NULL && p2 != NULL)
        return -1;
    else if (p1 != NULL && p2 == NULL)
        return 1;
    #endif

    while ((result = tolower(*p1) - tolower(*p2++)) == 0) {
        if (*p1++ == '\0')
            break;
    }

    return result;
}
