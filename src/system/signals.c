/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

/**
 * @file signals.c
 * @date 2022-02-04
*/

#include <signal.h>
#include "shared.h"
#include "common.h"

void sys_exit(int);

static bool signalcaught = false;

static void signal_handler(int sig)
{
    if (signalcaught) {
        fprintf(stderr, "double signal fault: received signal %d, exiting...\n", 
                sig);
        sys_exit(1);
    }

    signalcaught = true;
    fprintf(stderr, "received signal %d, exiting...\n", sig);
    sys_exit(0);
}

void init_signals(void)
{
    signal(SIGILL, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGTERM, signal_handler);

    #ifdef __linux__
    signal(SIGHUP, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTRAP, signal_handler);
    signal(SIGIOT, signal_handler);
    signal(SIGBUS, signal_handler);
    #endif
}
