#include <stdarg.h>
#include <stdio.h>

#include "iocod.h"
#include "windows_local.h"

extern void com_quit_f(void);

void DECL sys_error(const char *error, ...)
{
    va_list argptr;
    char text[4096];
    MSG msg;

    va_start(argptr, error);
    vsprintf(text, error, argptr);
    va_end(argptr);

    console_append(text);
    console_append("\n");

    set_error_text(text);
    console_show(1, true);

    timeEndPeriod(1);

    // in_shutdown();

    while (true) {
        if (!GetMessage(&msg, NULL, 0, 0))
            com_quit_f();

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    console_destroy();

    exit(1);
}
