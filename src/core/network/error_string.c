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

#include "net_local.h"

#ifdef IC_PLATFORM_WINDOWS
static char *get_wsa_error_string(int code)
{
    static wchar_t wmsg[1024] = { 0 };
    static char msg[1024] = { 0 };

    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  wmsg, sizeof(wmsg), NULL);

    utf16_shorten(wmsg, msg);

    if (*msg == '\0')
        snprintf(msg, sizeof(msg), "WSAGetLastError: %d", code);

    return msg;
}
#endif

char *net_error_string(void)
{
    int code;

    #ifdef IC_PLATFORM_WINDOWS
    code = WSAGetLastError();
    return get_wsa_error_string(code);
    #else
    code = errno;
    return strerror(code);
    #endif
}
