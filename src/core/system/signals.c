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
#include <signal.h>
#else
#include <sys/signal.h>
#include <err.h>
#endif

#ifdef IC_PLATFORM_WINDOWS
static const char *get_signal_text(int sig)
#else
static const char *get_signal_text(int sig, siginfo_t *siginfo, void *context)
#endif
{
    switch (sig) {
    case SIGSEGV:
        return "SIGSEGV: illegal storage access";
    case SIGABRT:
        return "SIGABRT: abnormal termination";
    case SIGINT:
        return "SIGINT: interrupt request";
    case SIGTERM:
        return "SIGTERM: termination request";
    case SIGFPE:
        #ifdef IC_PLATFORM_WINDOWS
        return "SIGFPE: floating-point error";
        #else
        switch (siginfo->si_code) {
        case FPE_INTDIV:
            return "SIGFPE: integer divide by zero";
        case FPE_INTOVF:
            return "SIGFPE: integer overflow";
        case FPE_FLTDIV:
            return "SIGFPE: floating-point divide by zero";
        case FPE_FLTOVF:
            return "SIGFPE: floating-point overflow";
        case FPE_FLTUND:
            return "SIGFPE: floating-point underflow";
        case FPE_FLTRES:
            return "SIGFPE: floating-point inexact result";
        case FPE_FLTINV:
            return "SIGFPE: floating-point invalid operation";
        case FPE_FLTSUB:
            return "SIGFPE: subscript out of range";
        default:
            return "SIGFPE: arithmetic exception";
        }
        break;
        #endif
    case SIGILL:
        #ifdef IC_PLATFORM_WINDOWS
        return "SIGILL: illegal instruction";
        #else
        switch (siginfo->si_code) {
        case ILL_ILLOPC:
            return "SIGILL: illegal opcode";
        case ILL_ILLOPN:
            return "SIGILL: illegal operand";
        case ILL_ILLADR:
            return "SIGILL: illegal addressing mode";
        case ILL_ILLTRP:
            return "SIGILL: illegal trap";
        case ILL_PRVOPC:
            return "SIGILL: privileged opcode";
        case ILL_PRVREG:
            return "SIGILL: privileged register";
        case ILL_COPROC:
            return "SIGILL: coprocessor error";
        case ILL_BADSTK:
            return "SIGILL: internal stack error";
        default:
            return "SIGILL: illegal instruction";
        }
        break;
        #endif
    default:
        return NULL;
    }
}

#ifdef IC_PLATFORM_WINDOWS
// on Windows this is non-static cause we need it on con_init() to set up
// proper SIGINT handling
void sys_signal_handler(int signal)
#else
static void sys_signal_handler(int signal, siginfo_t *siginfo, void *context)
#endif
{
    static qbool caught = false;
    static qbool caught_sigint = false;
    static int last_sigint = 0;

    #ifdef IC_PLATFORM_WINDOWS
    const char *sigtext = get_signal_text(signal);
    #else
    const char *sigtext = get_signal_text(signal, siginfo, context);
    #endif

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
            log_warn(_("Double signal fault: received %d (%s), ""exiting..."),
                     signal, sigtext);
        } else {
            caught = true;

            // cl shutdown
            // sv shutdown
        }
    }

quit:
    log_error(_("Signal %d (%s) caught, terminating..."), signal, sigtext);

    // dump backtrace
    if (signal == SIGSEGV) {
        log_error(_("Check crash.log for details"));
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
    #ifdef IC_PLATFORM_WINDOWS
    signal(SIGILL, sys_signal_handler);
    signal(SIGFPE, sys_signal_handler);
    signal(SIGSEGV, sys_signal_handler);
    signal(SIGTERM, sys_signal_handler);
    signal(SIGABRT, sys_signal_handler);

    // according to MSDN:
    // SIGINT is not supported for any Win32 application. 
    // When a CTRL+C interrupt occurs, Win32 operating systems 
    // generate a new thread to specifically handle that interrupt.
    #else
    uint8_t altstack[SIGSTKSZ];
    {
        stack_t ss = { 0 };
        ss.ss_sp = (void *) altstack;
        ss.ss_size = SIGSTKSZ;
        ss.ss_flags = 0;

        if (sigaltstack(&ss, NULL) != 0)
            err(1, "sigaltstack");
    }

    {
        struct sigaction sig_action;
        sig_action.sa_sigaction = sys_signal_handler;
        sigemptyset(&sig_action.sa_mask);

        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;

        if (sigaction(SIGSEGV, &sig_action, NULL) != 0)
            err(1, "sigaction");

        if (sigaction(SIGFPE, &sig_action, NULL) != 0)
            err(1, "sigaction");

        if (sigaction(SIGINT, &sig_action, NULL) != 0)
            err(1, "sigaction");

        if (sigaction(SIGILL, &sig_action, NULL) != 0)
            err(1, "sigaction");

        if (sigaction(SIGTERM, &sig_action, NULL) != 0)
            err(1, "sigaction");

        if (sigaction(SIGABRT, &sig_action, NULL) != 0)
            err(1, "sigaction");
    }
    #endif
}
