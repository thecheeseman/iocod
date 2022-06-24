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

static void print_gpl(void)
{
    con_print("iocod copyright (C) 2022 thecheeseman\n"
              "iocod comes with ABSOLUTELY NO WARRANTY; for details use the "
              "command 'gplinfo'.\nThis is free software, and you are welcome "
              "to redistribute it under certain\nconditions; use 'gplinfo' "
              "for details.\n\n");
}

IC_PUBLIC
int main(int argc, char *argv[])
{
    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    con_init();
    con_set_title(IC_CONSOLE_TITLE);

    print_gpl();

    config_init();
    log_init();

    // autoupdate?
    sys_setup_signal_handler();
    sys_set_floatenv();
    sys_milliseconds();
    sys_platform_init();

    // parse args
    // argc/argv
    
    com_init();
    net_init();
    
    while (true) {
        com_frame();
    }

    sys_quit();
    return 0;
}
