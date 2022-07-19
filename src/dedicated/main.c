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
#include <float.h>
#include <stdlib.h>
#include <locale.h>

static void print_gpl(void)
{
    ic_printf("iocod copyright (C) 2022 thecheeseman\n"
              "iocod comes with ABSOLUTELY NO WARRANTY; for details use the "
              "command 'gplinfo'.\nThis is free software, and you are welcome "
              "to redistribute it under certain\nconditions; use 'gplinfo' "
              "for details.\n\n");
}

static void parse_args(int argc, char *argv[])
{
    if (argc != 2)
        return;

    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        fprintf(stderr, IC_VERSION_STRING_FULL " " IC_PLATFORM_STRING "\n");
        exit(0);
    }
}

static void concat_args(int argc, char *argv[], size_t size, char *cmdline)
{
    for (int i = 1; i < argc; i++) {
        qbool spaces = (strchr(argv[i], ' ') != NULL);
        if (spaces)
            strncatz(cmdline, size, "\"");

        strncatz(cmdline, size, argv[i]);

        if (spaces)
            strncatz(cmdline, size, "\"");

        strncatz(cmdline, size, " ");
    }
}

IC_PUBLIC
int main(int argc, char *argv[])
{
    #ifdef IC_PLATFORM_WINDOWS
    setlocale(LC_ALL, ".UTF-8");
    #else
    setlocale(LC_ALL, "C.UTF-8");
    #endif

    // setup signals
    sys_setup_signal_handler();

    // parse args
    parse_args(argc, argv);

    // argc/argv
    char cmdline[MAX_STRING_CHARS] = { 0 };
    concat_args(argc, argv, sizeof(cmdline), cmdline);

    print_gpl();

    config_init();
    con_init();
    con_set_title(IC_CONSOLE_TITLE);
    log_init();

    // set the initial time base
    sys_milliseconds();
    
    com_init(cmdline);

    while (true) {
        com_frame();
    }

    // never reached
    sys_quit();
    return 0;
}
