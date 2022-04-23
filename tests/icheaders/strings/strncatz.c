#include "../iocod.h"

IC_PUBLIC
IC_NON_NULL(1, 3)
void strncatz(char *dest, int size, const char *src)
{
    int l1 = strlen(dest);
    if (l1 >= size)
        return; // com_error(ERR_FATAL, "strncatz: already overflowed");

    strncpyz(dest + l1, src, size - l1);
}
