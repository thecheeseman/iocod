#include "iocod.h"

#include <stdio.h>

#include "common/error.h"
#include "common/memory.h"
#include "strings/stringlib.h"
#include "va.h"

char *va(char *format, ...)
{
    va_list argptr;
    #define MAX_VA_STRING 32000
    static char temp_buffer[MAX_VA_STRING];
    static char string[MAX_VA_STRING];
    static int index = 0;
    char *buf;
    int len;

    va_start(argptr, format);
    vsprintf(temp_buffer, format, argptr);
    va_end(argptr);

    if ((len = strlen(temp_buffer)) >= MAX_VA_STRING)
        com_error(ERR_DROP, "Attempted to overrun string in call to va()\n");

    if (len + index >= MAX_VA_STRING - 1)
        index = 0;

    buf = &string[index];
    com_memcpy(buf, temp_buffer, len + 1);
    index += len + 1;
    return buf;
}
