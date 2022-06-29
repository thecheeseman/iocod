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

struct cvar *com_busy_wait;
struct cvar *com_dedicated;
struct cvar *com_developer;
struct cvar *com_sv_running;

IC_PUBLIC
void com_init(char *cmdline)
{
    ic_print_header("Common", 40, '-');

    // setjmp
    
    UNUSED_PARAM(cmdline);
    
    ev_init();
    // rand_init
    cv_init();

    // parse command line
    
    cbuf_init();
    
    // startup variable

    cmd_init();

    #ifdef IC_DEBUG
    com_developer = cv_get("developer", "1", CV_TEMP);
    #else
    com_developer = cv_get("developer", "0", CV_TEMP);
    #endif
    cv_set_description(com_developer, 
                       _("Developer mode. Prints out debug messages."));

    // base path
    // home path

    // filesystem init

    com_add_commands();

    // startup variable again

    com_dedicated = cv_get("dedicated", "2", CV_ROM);
    cv_set_description(com_dedicated, 
                       _("Dedicated server. TEMP: Forced to 2."));

    com_sv_running = cv_get("sv_running", "1", CV_ROM);
    cv_set_description(com_sv_running, 
                       _("Server running. TEMP: Forced to 1."));

    // init hunk

    // if any archived cvars are modified after this, trigger a config write
    cv_modified_flags &= ~CV_ARCHIVE;

    com_busy_wait = cv_get("com_busywait", "0", CV_ARCHIVE);

    ic_print_header("Common", 40, '-');
}

IC_PUBLIC
void com_shutdown(void)
{
    
}
