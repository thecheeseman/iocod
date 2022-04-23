#include "../iocod.h"

IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strndup(const char *s, size_t n)
{
    size_t len = strnlen(s, n);
    char *new = ic_malloc(len + 1);

    if (new == NULL)
        return NULL;

    new[len] = '\0';
    return (char *) memcpy(new, s, len);
}
