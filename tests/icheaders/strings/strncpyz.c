#include "../iocod.h"

IC_PUBLIC
void strncpyz(char *dest, const char *src, int destsize)
{
    if (!src)
        return; // com_error(ERR_FATAL, "strncpyz: NULL src");

    if (destsize < 1)
        return; // com_error(ERR_FATAL, "strncpyz: destsize < 1");

    strncpy(dest, src, destsize - 1);
    dest[destsize - 1] = 0;
}
