#include <ctype.h>
#include "../iocod.h"

IC_PUBLIC
IC_NON_NULL(1, 2)
int _ic_strncasecmp(const char *s1, const char *s2, size_t n)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    int result;

    if (p1 == p2 || n == 0)
        return 0;

    while ((result = tolower(*p1) - tolower(*p2++)) == 0) {
        if (*p1++ == '\0' || --n == 0)
            break;
    }

    return result;
}
