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

#include <errno.h>
#include <float.h>
#include <fcntl.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <conio.h>

#include "shared.h"
#include "common.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "system/shared.h"
#include "system/windows/console.h"
#include "system/windows/local.h"

struct cvar *tty_colors;

struct window_vars window_vars;
static char sys_cmdline[MAX_STRING_CHARS];

void init_signals(void); // signals.c

void gpl_notice(void)
{
    fprintf(stderr, 
            "iocod, copyright(C) 2021 - 2022 thecheeseman\n" \
            "iocod comes with ABSOLUTELY NO WARRANTY; " \
            "for details use the command 'gplinfo'.\n" \
            "This is free software, andyou are welcome to " \
            "redistribute it under certain conditions; use 'gplinfo' " \
            "for details.\n");
}

int total_msec, count_msec;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    char cwd[MAX_OSPATH];
    int start_time, end_time;

    if (hPrevInstance != NULL)
        return 0;

    window_vars.instance = hInstance;
    q_strncpyz(sys_cmdline, lpCmdLine, sizeof(sys_cmdline));

    console_create();

    SetErrorMode(SEM_FAILCRITICALERRORS);

    sys_milliseconds();

    com_init(sys_cmdline);
    net_init();

    (void) _getcwd(cwd, sizeof(cwd));
    com_printf("working directory: %s\n", cwd);

    console_show(0, false);

    SetFocus(window_vars.window);

    while (true) {
        if (window_vars.is_minimized ||
            com_dedicated != NULL && com_dedicated->integer > 0)
            Sleep(5);

        start_time = sys_milliseconds();
        
        // in_frame();

        com_frame();

        end_time = sys_milliseconds();
        total_msec += end_time - start_time;
        count_msec++;
    }

    // never gets here
    return 0;
}
