#include "iocod.h"
#include <stdlib.h>

#define MAX_PRINT_LEN 1024

IC_PUBLIC
IC_PRINTF_FORMAT(2, 3)
void _ic_print(bool warning, const char *fmt, ...)
{
    char msg[MAX_PRINT_LEN];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    if (warning) {
        #if !defined SYS_WARN
        fprintf(stderr, "WARNING: %s", msg);
        #else
        SYS_WARN(msg);
        #endif
    } else {
        #if !defined SYS_PRINT
        fprintf(stderr, "%s", msg);
        #else
        SYS_PRINT(msg);
        #endif
    }
}

IC_PUBLIC
IC_PRINTF_FORMAT(5, 6)
void _ic_error(bool fatal, const char *filename, const char *function, 
               const int line, const char *fmt, ...)
{
    char msg[MAX_PRINT_LEN];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    if (fatal) {
        #if !defined COM_ERROR
        fprintf(stderr, "ERROR: %s", msg);
        exit(0);
        #else
        COM_ERROR(ERR_FATAL, msg);
        #endif
    } else {
        #if !defined COM_ERROR
        fprintf(stderr, "ERROR: %s", msg);
        #else
        COM_ERROR(ERR_DROP, msg);
        #endif
    }
}
