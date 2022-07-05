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
bool fully_initialized = false;

struct cvar *com_busy_wait;
struct cvar *com_dedicated;
struct cvar *com_developer;
struct cvar *com_developer_script;
struct cvar *com_sv_running;
struct cvar *com_maxfps;
struct cvar *com_timescale;
struct cvar *com_fixedtime;
struct cvar *com_speeds;
struct cvar *com_cl_running;
struct cvar *cl_paused;
struct cvar *sv_paused;
struct cvar *com_version;
struct cvar *com_shortversion;
struct cvar *com_protocol;
struct cvar *com_legacy_protocol;
struct cvar *com_logfile;
struct cvar *com_viewlog;
struct cvar *protocol;

// common cvars
static void init_common_cvars(void)
{
    //
    // developer
    //
    #ifdef IC_DEBUG
    com_developer = cv_get("developer", "1", CV_TEMP);
    #else
    com_developer = cv_get("developer", "0", CV_TEMP);
    #endif
    cv_set_description(com_developer,
                       _("Developer mode. Prints out debug messages."));

    #ifdef IC_DEBUG
    com_developer_script = cv_get("developer_script", "2", CV_TEMP);
    #else
    com_developer_script = cv_get("developer_script", "0", CV_TEMP);
    #endif
    cv_set_description(com_developer_script,
                       _("Developer mode (script engine)."));

    //
    // dedicated
    //
    com_dedicated = cv_get("dedicated", "2", CV_ROM);
    cv_set_description(com_dedicated,
                       _("Dedicated server. TEMP: Forced to 2."));

    com_sv_running = cv_get("sv_running", "1", CV_ROM);
    cv_set_description(com_sv_running,
                       _("Server running. TEMP: Forced to 1."));

    // TEMP because no client
    com_cl_running = cv_get("cl_running", "0", CV_ROM);
    cl_paused = cv_get("cl_paused", "0", CV_ROM);
    sv_paused = cv_get("sv_paused", "0", CV_ROM);

    //
    // if any archived cvars are modified after this, trigger a config write
    //
    cv_modified_flags &= ~CV_ARCHIVE;

    com_maxfps = cv_get("com_maxfps", "85", CV_ARCHIVE);
    com_viewlog = cv_get("viewlog", "0", CV_CHEAT);
    com_logfile = cv_get("logfile", "0", CV_TEMP);
    cv_set_description(com_logfile,
                       _("Enable game logging. 1 = normal, 2 = flush."));

    com_timescale = cv_get("timescale", "1", CV_CHEAT | CV_SYSTEM_INFO);
    cv_set_description(com_timescale,
                       _("Game timescale. Cheat protected."));

    com_fixedtime = cv_get("fixedtime", "0", CV_CHEAT);
    com_speeds = cv_get("com_speeds", "0", CV_NONE);
    com_busy_wait = cv_get("com_busywait", "0", CV_ARCHIVE);

    //
    // protocol / version
    //
    com_protocol = cv_get("com_protocol", va("%i", PROTOCOL_VERSION),
                          CV_ROM | CV_SERVER_INFO);
    com_legacy_protocol = cv_get("com_legacyprotocol", va("%i", PROTOCOL_LEGACY),
                                 CV_ROM | CV_SERVER_INFO);

    if (com_legacy_protocol->integer > 0)
        protocol = cv_get("protocol", com_legacy_protocol->string, CV_ROM);
    else
        protocol = cv_get("protocol", com_protocol->string, CV_ROM);

    char *v = IC_VERSION_STRING_FULL " " IC_PLATFORM_STRING;
    com_version = cv_get("version", v, CV_ROM | CV_SERVER_INFO);
    cv_set_description(com_version, _("Full server version info."));

    // emulate
    if (com_legacy_protocol->integer > 0) {
        com_shortversion = cv_get("shortversion", va("1.%i", PROTOCOL_LEGACY),
                                  CV_ROM | CV_SERVER_INFO);
    } else {
        com_shortversion = cv_get("shortversion", 
                                  va("1.%s", com_protocol->string),
                                  CV_ROM | CV_SERVER_INFO);
    }

    // viewlog by default is on for dedicated
    if (com_dedicated->integer > 0 && com_viewlog->integer == 0)
        cv_set_integer("viewlog", 1);
}

IC_PUBLIC
void com_init(char *cmdline)
{
    ic_print_header("Common", 40, '-');
    ic_printf(IC_VERSION_STRING_FULL " " IC_PLATFORM_STRING "\n");

    if (setjmp(abortframe))
        sys_error("Error occurred during init");

    //
    // general init
    //
    ev_init();
    rand_init();
    cv_init();
    parse_command_line(cmdline);
    cbuf_init();
    cmd_init();

    // override anything from the config files with command line arguments
    startup_variable(NULL);
    startup_variable("developer");

    cl_init_key_commands();
    // fs_init();
    // com_init_journaling();

    //
    // exec configs
    //
    cbuf_add_execute("exec default_mp.cfg\n");
    cbuf_add_execute("exec language.cfg\n");
    cbuf_add_execute("exec config_mp.cfg\n");
    cbuf_add_execute("exec autoexec_mp.cfg\n");

    if (safe_mode())
        cbuf_add_execute("exec safemode_mp.cfg\n");

    // clear command buffer
    cbuf_execute();

    // recommended set
    // not sure if this needs to be implemented

    // startup variable again
    startup_variable(NULL);

    // seh_update_language_info();
    hunk_init();
    init_common_cvars();
    add_common_commands();

    //
    // system
    //
    sys_init();
    
    //netchan_init(com_milliseconds() & 0xffff);
    //script_init();
    //xanim_init();
    //dobj_init();
    //vm_init();
    sv_init();

    com_dedicated->modified = false;
    if (com_dedicated->integer == 0) {
        cl_init();
        // show console 0
    }

    com_frame_time = com_milliseconds();

    add_startup_commands();
    // cv_set_integer("r_uiFullscreen", 1);
    cl_start_hunk_users();

    // show console 0 again?

    fully_initialized = true;

    ic_print_header("Common", 40, '-');
}

IC_PUBLIC
void com_shutdown(void)
{
    cl_disconnect(true);
    cl_shutdown_all();
    sv_shutdown();
    cmd_shutdown();
    cv_shutdown();
}
