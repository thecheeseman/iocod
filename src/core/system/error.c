/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#else

#endif

IC_PUBLIC
void sys_error(const char *err, ...)
{
    va_list argptr;
    char str[1024];

    va_start(argptr, err);
    vsnprintf(str, sizeof(str), err, argptr);
    va_end(argptr);

    ic_printf("^1");
    ic_print_header("FATAL ERROR", 40, '*');
    log_error(str);
    ic_printf("^1");
    ic_print_header("", 40, '*');

    #ifdef IC_PLATFORM_WINDOWS
    wchar_t wstr[1024] = { 0 };
    wchar_t title[128] = { 0 };
    wchar_t prompt[128] = { 0 };
    wchar_t wmsg[1024] = { 0 };

    utf8_widen(_("An unrecoverable error has occurred: %s"), prompt);
    utf8_widen(_("Unrecoverable Error"), title);
    utf8_widen(str, wstr);
    _snwprintf(wmsg, sizeof(wmsg), prompt, wstr);
    MessageBoxW(NULL, wmsg, title, MB_OK | MB_ICONERROR);
    #endif

    sys_exit(IC_TERMINATE);
}
