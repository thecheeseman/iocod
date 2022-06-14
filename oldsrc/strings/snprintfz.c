#include <stdarg.h>
#include <stdio.h>
#include "strings/stringlib.h"

void snprintfz(char *dest, size_t size, const char *fmt, ...)
{
    size_t len;
    va_list argptr;

    va_start(argptr, fmt);
    vsnprintf(dest, size, fmt, argptr);
    va_end(argptr);
    dest[size - 1] = '\0';
}
