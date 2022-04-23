#include <stdio.h>
#include "../iocod.h"

IC_PUBLIC
IC_PRINTF_FORMAT(3, 4)
IC_NON_NULL(1)
int snprintfz(char *dest, size_t size, const char *fmt, ...)
{
    size_t len;
    va_list argptr;

    if (dest == NULL || size == 0 || fmt == NULL)
        return -1;

    va_start(argptr, fmt);
    len = vsnprintf(dest, size, fmt, argptr);
    va_end(argptr);
    dest[size - 1] = '\0';

    return len;
}
