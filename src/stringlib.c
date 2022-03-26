#include <ctype.h>
#include "stringlib.h"

#if !defined(HAVE_STRDUP)
#include <stdlib.h>

char *strdup(const char *str)
{
    int len;
    char *buf;

    if ((char *) str == NULL)
        return NULL;

    len = strlen(str) + 1;
    buf = malloc(len);

    if (buf != NULL) {
        memset(buf, 0, len);
        memcpy(buf, str, len - 1);
    }

    return buf;
}
#endif /* !defined(HAVE_STRDUP) */

#if !defined(HAVE_STRCASECMP)
int strcasecmp(const char *s1, const char *s2)
{
    const unsigned char *u1 = (const unsigned char *) s1;
    const unsigned char *u2 = (const unsigned char *) s2;
    int result;

    while ((result = tolower(*u1) - tolower(*u2)) == 0 && *u1 != '\0') {
        *u1++;
        *u2++;
    }

    return result;
}
#endif /* !defined(HAVE_STRCASECMP) */

#if !defined(HAVE_STRNCASECMP)
int strncasecmp(const char *s1, const char *s2, size_t n)
{
    const unsigned char *u1 = (const unsigned char *) s1;
    const unsigned char *u2 = (const unsigned char *) s2;
    int result;

    for (; n != 0; n--) {
        result = tolower(*u1) - tolower(*u2);

        if (result != 0)
            return result;

        if (*u1 == '\0')
            return 0;
    }

    return 0;
}
#endif /* !defined(HAVE_STRNCASECMP) */
