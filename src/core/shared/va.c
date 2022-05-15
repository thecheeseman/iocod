#include "iocod.h"

/* 
    this is smaller than RTCW (32000) but I can't imagine a scenario 
    where you actively need to concatenate more than 8K bytes at once
*/
#define MAX_VA_STRING 8192

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
char *va(const char *fmt, ...)
{
    va_list argptr;

    static char buf[MAX_VA_STRING];
    static char str[MAX_VA_STRING]; /* in case va is called by nested funcs */
    static size_t index = 0;

    va_start(argptr, fmt);
    vsnprintf(buf, sizeof(buf), fmt, argptr);
    va_end(argptr);

    size_t len = strnlen(buf, MAX_VA_STRING - 1);
    if (len + index >= MAX_VA_STRING - 1)
        index = 0;

    char *ptr = &str[index];
    memcpy(ptr, buf, len + 1);
    index += len + 1;

    return ptr;
}
