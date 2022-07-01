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
#include <signal.h>

static const char *signal_str[] = {
    [SIGABRT] = "abnormal termination",
    [SIGFPE] = "floating-point error",
    [SIGINT] = "interrupt request",
    [SIGILL] = "illegal instruction",
    [SIGSEGV] = "illegal storage access",
    [SIGTERM] = "termination request",

    #ifndef IC_PLATFORM_WINDOWS
    [SIGBUS] = "bus error",
    [SIGHUP] = "hangup",
    [SIGQUIT] = "quit",
    [SIGTRAP] = "trace trap",
    #endif

    // [SIGKILL] = "killed", // can't be handled by any process anyway
};
size_t signal_str_size = ARRAY_SIZE(signal_str);

IC_PUBLIC
void sys_signal_handler(int signal)
{
    static bool caught = false;
    static bool caught_sigint = false;
    static int32_t last_sigint = 0;

    const char *sigtext = NULL;
    if (signal <= (int) signal_str_size && signal > 0 && signal_str[signal] != NULL)
        sigtext = signal_str[signal];

    if (signal == SIGINT) {
        if (!caught_sigint || sys_milliseconds() - last_sigint > 3000) {
            caught_sigint = true;
            last_sigint = sys_milliseconds();

            con_print(_("Press Ctrl-C again within 3 seconds to exit\n"));
            return;
        }

        if (sys_milliseconds() - last_sigint < 3000)
            goto quit;
    } else {
        if (caught) {
            log_warn(_("Double signal fault: received %d (%s), ""exiting...\n"),
                     signal, sigtext);
        } else {
            caught = true;

            // cl shutdown
            // sv shutdown
        }
    }

quit:
    log_error(_("Signal %d (%s) caught, terminating...\n"), signal, sigtext);

    // dump backtrace
    if (signal == SIGSEGV) {
        log_error(_("Check crash.log for details\n"));
        sys_backtrace();
    }

    if (signal == SIGTERM || signal == SIGINT)
        sys_exit(IC_TERMINATE);
    else if (signal == SIGSEGV)
        sys_exit(IC_SEGFAULT);
    else
        sys_exit(IC_SIGNAL);
}

IC_PUBLIC
void sys_setup_signal_handler(void)
{
    signal(SIGILL, sys_signal_handler);
    signal(SIGFPE, sys_signal_handler);
    signal(SIGSEGV, sys_signal_handler);
    signal(SIGTERM, sys_signal_handler);
    signal(SIGABRT, sys_signal_handler);

    // according to MSDN:
    // SIGINT is not supported for any Win32 application. 
    // When a CTRL+C interrupt occurs, Win32 operating systems 
    // generate a new thread to specifically handle that interrupt.
    #ifndef IC_PLATFORM_WINDOWS
    signal(SIGINT, sys_signal_handler);
    #endif

    // other *nix signals
    #ifndef IC_PLATFORM_WINDOWS
    signal(SIGHUP, sys_signal_handler);
    signal(SIGQUIT, sys_signal_handler);
    signal(SIGTRAP, sys_signal_handler);
    signal(SIGIOT, sys_signal_handler);
    signal(SIGBUS, sys_signal_handler);
    #endif
}
