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

jmp_buf abortframe;

struct cvar *com_busy_wait;
struct cvar *com_dedicated;
struct cvar *com_developer;
struct cvar *com_sv_running;

struct cvar *com_timescale;
struct cvar *com_fixedtime;
struct cvar *com_speeds;

struct cvar *cl_paused;
struct cvar *sv_paused;

struct cvar *com_version;
struct cvar *com_protocol;
struct cvar *com_legacy_protocol;
struct cvar *com_logfile;
struct cvar *protocol;

IC_PUBLIC
void com_init(char *cmdline)
{
    ic_print_header("Common", 40, '-');

    if (setjmp(abortframe))
        sys_error("Error occured during init");

    ev_init();
    rand_init();
    cv_init();

    // parse command line
    parse_command_line(cmdline);

    cbuf_init();

    // startup variable -- override anything from the config files
    // with command line arguments
    startup_variable(NULL);

    // initialize commands
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
    startup_variable(NULL);

    com_dedicated = cv_get("dedicated", "2", CV_ROM);
    cv_set_description(com_dedicated,
                       _("Dedicated server. TEMP: Forced to 2."));

    // init hunk

    // if any archived cvars are modified after this, trigger a config write
    cv_modified_flags &= ~CV_ARCHIVE;

    //
    // cvars
    //
    com_logfile = cv_get("logfile", "0", CV_TEMP);
    cv_set_description(com_logfile,
                       _("Enable game logging. 1 = normal, 2 = flush."));

    com_timescale = cv_get("timescale", "1", CV_CHEAT | CV_SYSTEM_INFO);
    cv_set_description(com_timescale,
                       _("Game timescale. Cheat protected."));

    com_fixedtime = cv_get("fixedtime", "0", CV_CHEAT);

    cl_paused = cv_get("cl_paused", "0", CV_ROM);
    sv_paused = cv_get("sv_paused", "0", CV_ROM);

    com_speeds = cv_get("com_speeds", "0", CV_NONE);

    com_sv_running = cv_get("sv_running", "1", CV_ROM);
    cv_set_description(com_sv_running,
                       _("Server running. TEMP: Forced to 1."));

    com_busy_wait = cv_get("com_busywait", "0", CV_ARCHIVE);

    char *v = "iocod " IC_VERSION_STRING " " IC_PLATFORM_STRING " " __DATE__;
    com_version = cv_get("version", v, CV_ROM | CV_SERVER_INFO);

    com_protocol = cv_get("com_protocol", va("%i", PROTOCOL_VERSION),
                          CV_ROM | CV_SERVER_INFO);
    com_legacy_protocol = cv_get("com_legacyprotocol", va("%i", PROTOCOL_LEGACY),
                                 CV_ROM | CV_SERVER_INFO);

    if (com_legacy_protocol->integer > 0)
        protocol = cv_get("protocol", com_legacy_protocol->string, CV_ROM);
    else
        protocol = cv_get("protocol", com_protocol->string, CV_ROM);

    // sys_init
    // netchan_init
    // vm_init
    sv_init();

    com_frame_time = com_milliseconds();

    // add startup commands
    add_startup_commands();

    ic_print_header("Common", 40, '-');
}

IC_PUBLIC
void com_shutdown(void)
{
    cmd_shutdown();
    cv_shutdown();
}
