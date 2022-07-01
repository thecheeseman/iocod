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

#include "com_local.h"

bool error_entered = false;
char error_message[MAX_PRINT_MSG];

IC_PUBLIC
IC_PRINTF_FORMAT(5, 6)
void _ic_error(enum errcode code, const char *filename, const char *function,
               const int line, const char *fmt, ...)
{
    if (error_entered)
        sys_error(_("Recursive error after: %s"), error_message);

    // if we are getting a solid stream of ERR_DROP, do an ERR_FATAL
    static int errcount = 0;
    static int lasttime = 0;
    int currenttime = sys_milliseconds();
    if (currenttime - lasttime < 100) {
        if (++errcount > 3)
            code = ERR_FATAL;
    } else {
        errcount = 0;
    }
    lasttime = currenttime;

    error_entered = true;
    cv_set_integer("com_errorcode", code);

    va_list argptr = { 0 };
    va_start(argptr, fmt);
    vsnprintf(error_message, sizeof(error_message), fmt, argptr);
    va_end(argptr);

    if (code != ERR_DISCONNECT) // err need cd doesn't matter
        cv_set_string("com_errormessage", error_message);

    char loc[MAX_PRINT_MSG];
    snprintfz(loc, sizeof(loc), "%s::%s() at line %d",
              ic_short_filename(filename), function, line);

    switch (code) {
    case ERR_DISCONNECT:
    case ERR_SERVER_DISCONNECT:
        error_entered = false;
        longjmp(abortframe, -1);
        break;
    case ERR_DROP:
        ic_print_header("Error", 40, '*');
        log_error("%s", error_message);
        log_print("%s", loc);
        ic_print_header("", 40, '*');

        // sv_shutdown
        // restart client
        // cl disconnect
        // cl flush mem
        // pureserversetloadedpaks

        error_entered = false;
        longjmp(abortframe, -1);
        break;
    case ERR_NEED_CD:
        // useless
        error_entered = false;
        longjmp(abortframe, -1);
        break;
    default:
        // cl_shutdown
        // sv_shutdown
        break;
    }

    com_shutdown();

    sys_error("%s (%s)", error_message, loc);
}
