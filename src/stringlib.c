#include <ctype.h>
#include <stdlib.h>
#include "stringlib.h"

#if !defined(HAVE_STRDUP)
char *strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    void *new = malloc(len);

    if (new == NULL)
        return NULL;

    return (char *) memcpy(new, s, len);
}
#endif /* !defined(HAVE_STRDUP) */

#if !defined(HAVE_STRNDUP)
char *strndup(const char *s, size_t n)
{
    size_t len = strnlen(s, n);
    char *new = malloc(len + 1);

    if (new == NULL)
        return NULL;

    new[len] = '\0';
    return (char *) memcpy(new, s, len);
}
#endif

#if !defined(HAVE_STRCASECMP)
int strcasecmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    int result;

    if (p1 == p2)
        return 0;

    while ((result = tolower(*p1) - tolower(*p2++)) == 0) {
        if (*p1++ == '\0')
            break;
    }

    return result;
}
#endif /* !defined(HAVE_STRCASECMP) */

#if !defined(HAVE_STRNCASECMP)
int strncasecmp(const char *s1, const char *s2, size_t n)
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
#endif /* !defined(HAVE_STRNCASECMP) */

void strncpyz(char *dest, const char *src, int destsize)
{
    if (!src)
        return; // com_error(ERR_FATAL, "strncpyz: NULL src");

    if (destsize < 1)
        return; // com_error(ERR_FATAL, "strncpyz: destsize < 1");

    strncpy(dest, src, destsize - 1);
    dest[destsize - 1] = 0;
}

void strncatz(char *dest, int size, const char *src)
{
    int l1 = strlen(dest);
    if (l1 >= size)
        return; // com_error(ERR_FATAL, "strncatz: already overflowed");

    strncpyz(dest + l1, src, size - l1);
}

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
