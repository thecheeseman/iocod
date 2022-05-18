#include "iocod.h"

extern struct parse_info *pi;

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_warning(const char *fmt, ...)
{
    va_list argptr;
    char str[1024];

    va_start(argptr, fmt);
    vsnprintf(str, sizeof(str), fmt, argptr);
    va_end(argptr);

    ic_warning("file '%s' line %lu: %s\n", pi->parse_file, pi->lines, str);
}

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_error(const char *fmt, ...)
{
    va_list argptr;
    char str[1024];

    va_start(argptr, fmt);
    vsnprintf(str, sizeof(str), fmt, argptr);
    va_end(argptr);

    ic_error("file '%s' line %lu: %s\n", pi->parse_file, pi->lines, str);
}
