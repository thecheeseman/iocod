#include "../iocod.h"

IC_PUBLIC
IC_NON_NULL(1, 2)
void strncpyz(char *dest, const char *src, int destsize)
{
    if (!src)
        return; // com_error(ERR_FATAL, "strncpyz: NULL src");

    if (destsize < 1)
        return; // com_error(ERR_FATAL, "strncpyz: destsize < 1");

    strncpy(dest, src, destsize - 1);
    dest[destsize - 1] = 0;
}
