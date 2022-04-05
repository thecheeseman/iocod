#include <stdlib.h>
#include "strings/stringlib.h"

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
