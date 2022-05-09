#include "m_module.h"
#include <stdio.h>
#include <string.h>

static m_ptr (M_DECL *syscall)(m_ptr command, ...) = 
    (m_ptr (M_DECL *)(m_ptr, ...)) - 1;

M_EXPORT
void module_entry(m_ptr (M_DECL *syscallptr)(m_ptr command, ...))
{
    syscall = syscallptr;
}

void m_printf(const char *fmt)
{
    syscall(M_PRINTF, fmt);
}

void m_dprintf(const char *fmt)
{
    syscall(M_DPRINTF, fmt);
}

void m_warning(const char *fmt)
{
    syscall(M_WARNING, fmt);
}

void m_dwarning(const char *fmt)
{
    syscall(M_DWARNING, fmt);
}

void m_error(const char *fmt)
{
    syscall(M_ERROR, fmt);
}

void m_error_fatal(const char *fmt)
{
    syscall(M_ERROR_FATAL, fmt);
}

const char *m_callback_name(int callback)
{
    return (const char *) syscall(M_CALLBACK_NAME, callback);
}

#define MAX_VA_STRING 0x1000
char *M_DECL va(const char *fmt, ...)
{
    va_list argptr;
    static char tmpbuf[MAX_VA_STRING];
    static char string[MAX_VA_STRING];
    static size_t index = 0;

    va_start(argptr, fmt);
    vsnprintf(tmpbuf, sizeof(tmpbuf), fmt, argptr);
    va_end(argptr);

    size_t len = strnlen(tmpbuf, MAX_VA_STRING);
    if (len + index >= MAX_VA_STRING - 1)
        index = 0;

    char *buf = &string[index];
    memcpy(buf, tmpbuf, len + 1);
    index += len + 1;

    return buf;
}
