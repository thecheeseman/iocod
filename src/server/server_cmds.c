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

#include "server_local.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "strings/stringlib.h"
#include "infostring.h"
#include "vm.h"

void sv_heartbeat_f(void)
{
    svs.next_heartbeat_time = -9999999;
}

void INCOMPLETE sv_kick_f(void)
{

}

void INCOMPLETE sv_banuser_f(void)
{

}

void INCOMPLETE sv_banclient_f(void)
{

}

void INCOMPLETE sv_clientkick_f(void)
{

}

void INCOMPLETE sv_status_f(void)
{

}

static void sv_serverinfo_f(void)
{
    com_printf("Server info settings:\n");
    info_print(cvar_info_string(CVAR_SERVER_INFO));
}

static void sv_systeminfo_f(void)
{
    com_printf("System info settings:\n");
    info_print(cvar_info_string(CVAR_SYSTEM_INFO));
}

void INCOMPLETE sv_dumpuser_f(void)
{

}

/**
 * @brief 
 * @param 
 * @todo finish writing function
*/
void INCOMPLETE sv_map_restart_f(void)
{
    char mapname[MAX_QPATH];
    bool keep_persistent;

    // make sure we aren't restarting twice in the same frame
    if (com_frame_time == sv.server_id)
        return;

    if (!com_sv_running->integer) {
        com_printf("server is not running\n");
        return;
    }

    if (com_timescale->value <= 0.0)
        cvar_set_value("timescale", 1.0);

    cvar_get("g_gametype", "dm", CVAR_SERVER_INFO | CVAR_LATCH);

    keep_persistent = vm_call(gvm, GAME_GET_PERSISTENT);
    if (!keep_persistent) {
        if (strcasecmp(sv.gametype, sv_gametype->string)) {
            com_printf("g_gametype variable change -- restarting\n");

            strncpyz(mapname, cvar_variable_string("mapname"), 
                       sizeof(mapname));
            sv_spawn_server(mapname);
            return;
        }

        if (sv_maxclients->modified) {
            com_printf("sv_maxclients variable change -- restarting\n");

            strncpyz(mapname, cvar_variable_string("mapname"),
                       sizeof(mapname));
            sv_spawn_server(mapname);
            return;
        }
    } else {
        cvar_set("g_gametype", sv.gametype);
    }

    sv_set_gametype();
    sv_reset_snapshot_data();

    // bunch of other stuff here
}

/**
 * @brief 
 * @param  
*/
void sv_map_f(void)
{
    char *cmd, *map, *fmt, expanded[MAX_QPATH], mapname[MAX_QPATH];
    bool cheat = false;

    map = cmd_argv(1);
    if (!map)
        return;

    if (strncasecmp(map, "mp/", 3) == 0 || strncasecmp(map, "mp\\", 3) == 0)
        fmt = "maps/%s.bsp";
    else
        fmt = "maps/mp/%s.bsp";

    snprintfz(expanded, sizeof(expanded), fmt, map);
    if (fs_read_file(expanded, NULL) == -1) {
        com_printf("can't find map %s\n", expanded);
        return;
    }

    // force latched values to get set
    cmd = cmd_argv(0);
    if (strcasecmp(cmd, "devmap") == 0)
        cheat = true;

    strncpyz(mapname, map, sizeof(mapname));

    if (com_sv_running->integer && com_timescale->value <= 0.0)
        cvar_set_value("timescale", 1.0);

    if (!com_sv_running->integer || strcasecmp(mapname, sv_mapname->string))
        sv_spawn_server(mapname);
    else
        sv_map_restart_f();

    if (cheat)
        cvar_set("sv_cheats", "1");
    else
        cvar_set("sv_cheats", "0");
}

void INCOMPLETE sv_maprotate_f(void)
{

}

void INCOMPLETE sv_gamecompletestatus_f(void)
{

}

void INCOMPLETE sv_killserver_f(void)
{

}

void sv_consay_f(void)
{
    char *p, text[MAX_STRING_CHARS];

    if (!com_sv_running->integer) {
        com_printf("Server is not running\n");
        return;
    }

    if (cmd_argc() < 2)
        return;

    strcpy(text, "console: ");
    p = cmd_args_from(1); // 1.5 change

    if (*p == '\"') {
        p++;
        p[strlen(p) - 1] = '\0';
    }

    strcat(text, p);

    sv_send_server_command(NULL, NULL, "h \"x15%s\"", text);
}

#ifdef CODVERSION1_5
void INCOMPLETE sv_tell_f(void)
{

}
#endif

void INCOMPLETE sv_scriptusage_f(void)
{

}

void INCOMPLETE sv_stringusage_f(void)
{

}

#ifdef CODVERSION1_5
void INCOMPLETE sv_tempbanuser_f(void)
{

}

void INCOMPLETE sv_tempbanclient_f(void)
{

}

void INCOMPLETE sv_unbanuser_f(void)
{

}

void INCOMPLETE sv_set_drawfriend_f(void)
{

}

void INCOMPLETE sv_set_friendlyfire_f(void)
{

}

void INCOMPLETE sv_set_killcam_f(void)
{

}
#endif

void sv_configstrings_f(void)
{
    int i;

    for (i = 0; i < MAX_CONFIG_STRINGS; i++) {
        if (!sv.configstrings[i] || !sv.configstrings[i][0])
            continue;

        com_printf("%4i: %s\n", i, sv.configstrings[i]);
    }
}

void sv_test_f(void)
{
    com_printf("sizeof(struct shared_entity) = %d\n", sizeof(struct shared_entity));
    com_printf("sizeof(struct entity_state) = %d\n", sizeof(struct entity_state));
    com_printf("sizeof(struct entity_shared) = %d\n", sizeof(struct entity_shared));
    com_printf("sizeof(struct player_state) = %d\n", sizeof(struct player_state));
}

void sv_add_operator_commands(void)
{
    static bool initialized;

    if (initialized)
        return;
    initialized = true;

    cmd_add_command("heartbeat", sv_heartbeat_f);
    cmd_add_command("kick", sv_kick_f);
    cmd_add_command("banuser", sv_banuser_f);
    cmd_add_command("banclient", sv_banclient_f);
    cmd_add_command("clientkick", sv_clientkick_f);
    cmd_add_command("status", sv_status_f);
    cmd_add_command("serverinfo", sv_serverinfo_f);
    cmd_add_command("systeminfo", sv_systeminfo_f);
    cmd_add_command("dumpuser", sv_dumpuser_f);
    cmd_add_command("map_restart", sv_map_restart_f);
    cmd_add_command("map", sv_map_f);
    cmd_add_command("map_rotate", sv_maprotate_f);
    cmd_add_command("gamecompletestatus", sv_gamecompletestatus_f);
    cmd_add_command("devmap", sv_map_f);
    cmd_add_command("killserver", sv_killserver_f);

    if (com_dedicated->integer) {
        cmd_add_command("say", sv_consay_f);

        #ifdef CODVERSION1_5
        cmd_add_command("tell", sv_tell_f);
        #endif
    }

    cmd_add_command("scriptusage", sv_scriptusage_f);
    cmd_add_command("stringusage", sv_stringusage_f);

    #ifdef CODVERSION1_5
    cmd_add_command("tempbanuser", sv_tempbanuser_f);
    cmd_add_command("tempbanclient", sv_tempbanclient_f);
    cmd_add_command("unbanuser", sv_unbanuser_f);
    cmd_add_command("setdrawfriend", sv_set_drawfriend_f);
    cmd_add_command("setfriendlyfire", sv_set_friendlyfire_f);
    cmd_add_command("setkillcam", sv_set_killcam_f);
    #endif

    cmd_add_command("configstrings", sv_configstrings_f);

    cmd_add_command("test", sv_test_f);
    cmd_add_alias("test", "test2");
}

void sv_remove_operator_commands(void)
{
    cmd_remove_command("heartbeat");
    cmd_remove_command("kick");
    cmd_remove_command("banuser");
    cmd_remove_command("banclient");
    cmd_remove_command("clientkick");
    cmd_remove_command("status");
    cmd_remove_command("serverinfo");
    cmd_remove_command("systeminfo");
    cmd_remove_command("dumpuser");
    cmd_remove_command("map_restart");
    cmd_remove_command("map");
    cmd_remove_command("map_rotate");
    cmd_remove_command("gamecompletestatus");
    cmd_remove_command("devmap");
    cmd_remove_command("killserver");

    cmd_remove_command("say");

    #ifdef CODVERSION1_5
    cmd_remove_command("tell");
    #endif
    cmd_remove_command("scriptusage");
    cmd_remove_command("stringusage");

    #ifdef CODVERSION1_5
    cmd_remove_command("tempbanuser");
    cmd_remove_command("tempbanclient");
    cmd_remove_command("unbanuser");
    cmd_remove_command("setdrawfriend");
    cmd_remove_command("setfriendlyfire");
    cmd_remove_command("setkillcam");
    #endif

    cmd_remove_command("configstrings");

    cmd_remove_command("test");
}
