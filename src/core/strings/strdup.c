#include <stdlib.h>
#include "iocod.h"

IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strdup(const char *s)
{
    #if !defined IC_GNUC_VERSION
    if (s == NULL)
        return NULL;
    #endif

    size_t len = strlen(s) + 1;
    void *new = ic_malloc(len);

    if (new == NULL)
        return NULL;

    return (char *) memcpy(new, s, len);
}
