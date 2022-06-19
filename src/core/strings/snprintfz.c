#include <stdio.h>
#include "iocod.h"

IC_PUBLIC
IC_PRINTF_FORMAT(3, 4)
IC_NON_NULL(1)
ssize_t snprintfz(char *dest, size_t size, const char *fmt, ...)
{
    #if !defined IC_GNUC_VERSION
    if (dest == NULL || fmt == NULL)
        return -1;
    #endif

    if (size == 0)
        return -1;

    va_list argptr;
    va_start(argptr, fmt);
    ssize_t len = vsnprintf(dest, size, fmt, argptr);
    va_end(argptr);
    dest[size - 1] = '\0';

    return len;
}
