#include "iocod.h"

IC_PUBLIC
IC_NON_NULL(1, 3)
void strncatz(char *dest, size_t size, const char *src)
{
    #if !defined IC_GNUC_VERSION
    if (dest == NULL || src == NULL)
        return;
    #endif

    size_t l1 = strlen(dest);
    if (l1 >= size)
        return; // com_error(ERR_FATAL, "strncatz: already overflowed");

    strncpyz(dest + l1, src, size - l1);
}
