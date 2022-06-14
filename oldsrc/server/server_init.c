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

#include <ctype.h>

#include "clipmap/clipmap.h"
#include "common/error.h"
#include "common/hunk.h"
#include "common/memory.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "server_local.h"
#include "strings/stringlib.h"
#include "va.h"
#include "vm.h"

void sv_set_config_string(int index, const char *val)
{
    int len, i;
    int maxchunksize = MAX_STRING_CHARS - 24;

    if (index < 0 || index >= MAX_CONFIG_STRINGS)
        com_error(ERR_DROP, "bad index %i", index);

    if (!val)
        val = "";

    // don't update if there's no change
    if (!strcmp(val, sv.configstrings[index]))
        return;

    z_free(sv.configstrings[index]);
    sv.configstrings[index] = copy_string(val);

    // send it to all the clients if we aren't spawning a new server
    if (sv.state == SS_GAME || sv.restarting) {
        struct client *cl;

        for (i = 0, cl = svs.clients; i < sv_maxclients->integer; i++, cl++) {
            if (cl->state < CS_PRIMED)
                continue;

            if (index == CS_SERVER_INFO && cl->gentity &&
                (cl->gentity->r.sv_flags & SVF_NOSERVERINFO))
                continue;

            len = strlen(val);
            if (len >= maxchunksize) {
                int sent = 0;
                int remaining = len;
                char *cmd;
                char buf[MAX_STRING_CHARS];

                while (remaining > 0) {
                    if (sent == 0)
                        cmd = "x";
                    else if (remaining < maxchunksize)
                        cmd = "z";
                    else
                        cmd = "y";

                    strncpyz(buf, &val[sent], maxchunksize);

                    sv_send_server_command(cl, "%s %i %s", cmd, index, buf);

                    sent += (maxchunksize - 1);
                    remaining -= (maxchunksize - 1);
                }
            } else {
                sv_send_server_command(cl, "d %i %s", index, val);
            }
        }
    }
}

void sv_get_config_string(int index, char *buffer, int bufsize)
{
    if (bufsize < 1)
        com_error(ERR_DROP, "bufsize == %i", bufsize);

    if (index < 0 || index >= MAX_CONFIG_STRINGS)
        com_error(ERR_DROP, "bad index %i", index);

    if (!sv.configstrings[index]) {
        buffer[0] = '\0';
        return;
    }

    strncpyz(buffer, sv.configstrings[index], bufsize);
}

void sv_init(void)
{
    sv_add_operator_commands();

    //
    // serverinfo vars
    //
    sv_gametype = cvar_get("g_gametype", "dm", CVAR_SERVER_INFO | CVAR_LATCH);

    cvar_get("sv_keywords", "", CVAR_SERVER_INFO);
    cvar_get("protocol", va("%i", PROTOCOL_VERSION), CVAR_SERVER_INFO | CVAR_ROM);

    sv_mapname = cvar_get("mapname", "nomap", CVAR_SERVER_INFO | CVAR_ROM);
    sv_privateclients = cvar_get("sv_privateclients", "0", CVAR_SERVER_INFO);
    sv_hostname = cvar_get("sv_hostname", "iocodhost", CVAR_SERVER_INFO | CVAR_ARCHIVE);
    sv_maxclients = cvar_get("sv_maxclients", "20", CVAR_SERVER_INFO | CVAR_LATCH);

    #ifdef CODVERSION1_5
    sv_punkbuster = cvar_get("sv_punkbuster", "0", CVAR_ROM | CVAR_SERVERINFO | CVAR_ARCHIVE);
    #endif

    sv_maxrate = cvar_get("sv_maxrate", "0", CVAR_ARCHIVE | CVAR_SERVER_INFO);
    sv_maxping = cvar_get("sv_maxping", "0", CVAR_ARCHIVE | CVAR_SERVER_INFO);
    sv_minping = cvar_get("sv_minping", "0", CVAR_ARCHIVE | CVAR_SERVER_INFO);
    sv_floodprotect = cvar_get("sv_floodprotect", "0", CVAR_ARCHIVE | CVAR_SERVER_INFO);

    sv_allowanonymous = cvar_get("sv_allowanonymous", "0", CVAR_SERVER_INFO);
    sv_showcommands = cvar_get("sv_showcommands", "0", 0);

    //
    // systeminfo vars
    //
    #ifdef CODVERSION1_5
    sv_disableclientconsole = cvar_get("sv_disableclientconsole", "0", CVAR_SYSTEM_INFO);
    #endif

    cvar_get("sv_cheats", "0", CVAR_SYSTEM_INFO | CVAR_ROM);
    
    sv_serverid = cvar_get("sv_serverid", "0", CVAR_SYSTEM_INFO | CVAR_ROM);
    sv_pure = cvar_get("sv_pure", "1", CVAR_SYSTEM_INFO | CVAR_SERVER_INFO);

    cvar_get("sv_paks", "", CVAR_SYSTEM_INFO | CVAR_ROM);
    cvar_get("sv_paknames", "", CVAR_SYSTEM_INFO | CVAR_ROM);
    cvar_get("sv_referencedpaks", "", CVAR_SYSTEM_INFO | CVAR_ROM);
    cvar_get("sv_referencedpaknames", "", CVAR_SYSTEM_INFO | CVAR_ROM);

    //
    // server vars
    //
    sv_rconpassword = cvar_get("rconpassword", "", CVAR_TEMP);
    sv_privatepassword = cvar_get("sv_privatepassword", "", CVAR_TEMP);
    sv_fps = cvar_get("sv_fps", "20", CVAR_TEMP);
    sv_timeout = cvar_get("sv_timeout", "240", CVAR_TEMP);
    sv_zombietime = cvar_get("sv_zombietime", "2", CVAR_TEMP);

    cvar_get("nextmap", "", CVAR_TEMP);

    sv_allowdownload = cvar_get("sv_allowdownload", "1", CVAR_ARCHIVE);

    sv_master[0] = cvar_get("sv_master1", MASTER_SERVER_NAME, 0);
    sv_master[1] = cvar_get("sv_master2", "", CVAR_ARCHIVE);
    sv_master[2] = cvar_get("sv_master3", "", CVAR_ARCHIVE);
    sv_master[3] = cvar_get("sv_master4", "", CVAR_ARCHIVE);
    sv_master[4] = cvar_get("sv_master5", "", CVAR_ARCHIVE);

    sv_reconnectlimit = cvar_get("sv_reconnectlimit", "3", 0);
    sv_showloss = cvar_get("sv_showloss", "0", 0);
    sv_padpackets = cvar_get("sv_padpackets", "0", 0);
    sv_killserver = cvar_get("sv_killserver", "0", 0);
    sv_onlyvisibleclients = cvar_get("sv_onlyvisibleclients", "0", 0);
    sv_showaveragebps = cvar_get("sv_showaveragebps", "0", 0);

    #ifdef CODVERSION1_5
    sv_kickbantime = cvar_get("sv_kickbantime", "300", 0);
    #endif

    cvar_get("g_complaintlimit", "3", CVAR_ARCHIVE);

    sv_maprotation = cvar_get("sv_maprotation", "", 0);
    sv_maprotation_current = cvar_get("sv_maprotationcurrent", "", 0);

    #ifdef CODVERSION1_5
    sv_wwwdownload = cvar_get("sv_wwwdownload", "0", CVAR_ARCHIVE);
    sv_wwwbaseurl = cvar_get("sv_wwwbaseurl", "", CVAR_ARCHIVE);
    sv_wwwdldisconnected = cvar_get("sv_wwwdldisconnected", "0", CVAR_ARCHIVE);
    #endif
}

void sv_bound_max_clients(int min)
{
    cvar_get("sv_maxclients", "20", 0);

    sv_maxclients->modified = false;

    if (sv_maxclients->integer < min)
        cvar_set("sv_maxclients", va("%i", min));
    else if (sv_maxclients->integer > MAX_CLIENTS)
        cvar_set("sv_maxclients", va("%i", MAX_CLIENTS));
}

void sv_change_max_clients(void)
{
    int old, i, count;
    struct client *oldclients;

    // get the highest client number in use
    count = 0;
    for (i = 0; i < sv_maxclients->integer; i++) {
        if (svs.clients[i].state >= CS_CONNECTED) {
            if (i > count)
                count = i;
        }
    }
    count++;

    old = sv_maxclients->integer;

    // never go below the highest client number in use
    sv_bound_max_clients(count);
    if (sv_maxclients->integer == old)
        return; // nothing changed

    oldclients = hunk_allocate_temp_memory(count * sizeof(struct client));

    // copy clients to hunk memory
    for (i = 0; i < count; i++) {
        if (svs.clients[i].state >= CS_CONNECTED)
            oldclients[i] = svs.clients[i];
        else
            memset(&oldclients[i], 0, sizeof(struct client));
    }
    z_free(svs.clients);

    // alloc new clients
    svs.clients = calloc(sv_maxclients->integer * sizeof(struct client), 1);
    if (!svs.clients)
        com_error(ERR_FATAL, "unable to allocate svs.clients");

    // copy clients over
    for (i = 0; i < count; i++) {
        if (oldclients[i].state >= CS_CONNECTED)
            svs.clients[i] = oldclients[i];
    }

    hunk_free_temp_memory(oldclients);

    // allocate new snapshot entities/clients
    if (com_dedicated->integer) {
        svs.num_snapshot_entities = sv_maxclients->integer << 12;
        svs.num_snapshot_clients = sv_maxclients->integer * 
            sv_maxclients->integer * 32;
    } else {
        svs.num_snapshot_entities = sv_maxclients->integer << 8;
        svs.num_snapshot_clients = sv_maxclients->integer *
            sv_maxclients->integer * 4;
    }
}

void sv_startup(void)
{
    if (svs.initialized)
        com_error(ERR_FATAL, "svs.initalized");

    sv_bound_max_clients(1);

    svs.clients = calloc(sv_maxclients->integer * sizeof(struct client), 1);
    if (!svs.clients)
        com_error(ERR_FATAL, "unable to allocate svs.clients");

    if (com_dedicated->integer) {
        svs.num_snapshot_entities = sv_maxclients->integer << 12;
        svs.num_snapshot_clients = sv_maxclients->integer * sv_maxclients->integer * 64;
    } else {
        svs.num_snapshot_entities = sv_maxclients->integer << 8;
        svs.num_snapshot_clients = sv_maxclients->integer * sv_maxclients->integer * 4;
    }

    svs.initialized = true;

    cvar_set("sv_running", "1");
}

void sv_clear_server(void)
{
    int i;

    for (i = 0; i < MAX_CONFIG_STRINGS; i++) {
        if (sv.configstrings[i])
            z_free(sv.configstrings[i]);
    }

    memset(&sv, 0, sizeof(sv));
}

void sv_set_gametype(void)
{
    char *gt, *p;

    gt = cvar_variable_string("g_gametype");
    strncpyz(sv.gametype, gt, sizeof(sv.gametype));

    for (p = sv.gametype; *p != '\0'; p++)
        *p = (char) tolower((int) *p);

    cvar_set("g_gametype", sv.gametype);

    cvar_update_flags();
}

void sv_reset_snapshot_data(void)
{
    svs.field_0x28 = 0;
    svs.next_archived_snapshot_frames = 0;
    svs.next_archived_snapshot_buffer = 0;
    svs.next_cached_snapshot_entities = 0;
    svs.next_cached_snapshot_clients = 0;
    svs.next_cached_snapshot_frames = 0;
}

void sv_create_baseline(void)
{
    struct shared_entity *svent;
    int i;

    for (i = 1; i < sv.num_entities; i++) {
        svent = sv_gentity_num(i);
        if (!svent->r.linked)
            continue;

        svent->s.number = i;

        sv.sv_entities[i].baseline = svent->s;
    }
}

void INCOMPLETE sv_spawn_server(char *server)
{
    int i, checksum;
    char system_info[16384];
    struct cvar *xmodelcheck UNUSED;
    bool keep_persistent;
    
    xmodelcheck = cvar_get("cl_xmodelcheck", "0", CVAR_LATCH | CVAR_ARCHIVE);
    // fun_080cc4b0(xmodelcheck->integer);

    cvar_get("g_gametype", "dm", CVAR_LATCH | CVAR_SERVER_INFO);

    // why is this a float?
    if (!cvar_variable_value("sv_running")) {
        keep_persistent = false;
    } else {
        keep_persistent = vm_call(gvm, GAME_GET_PERSISTENT);

        // update clients here
    }

    // empty 
    cl_map_loading();
    cl_shutdown_all();
    // empty

    // shutdown the existing game if its running
    sv_shutdown_game_progs();

    com_print_header("server initialization", 40, '-');
    com_printf("Server: %s\n", server);

    hunk_clear2();

    sv_clear_server();

    fs_shutdown(true);

    fs_clear_pak_references(0);

    srand(com_milliseconds());
    sv.checksum_feed = (((int) rand() << 16) ^ rand()) ^ com_milliseconds();
    fs_restart(sv.checksum_feed);

    // clear search paths
    // fun_08061980()

    // allocate blank strings for each configstring
    for (i = 0; i < MAX_CONFIG_STRINGS; i++)
        sv.configstrings[i] = copy_string("");

    // why is this a float?
    if (!cvar_variable_value("sv_running")) {
        // script_function()

        sv_startup();
    } else {
        // script_function()

        if (sv_maxclients->modified)
            sv_change_max_clients();
    }

    sv_set_gametype();

    //
    svs.snapshot_entities =
        hunk_alloc(sizeof(struct entity_state) * svs.num_snapshot_entities);
    svs.next_snapshot_entities = 0;
    svs.snapshot_clients =
        hunk_alloc(0x5c * svs.num_snapshot_clients);
    svs.next_snapshot_clients = 0;

    sv_reset_snapshot_data();

    svs.snap_flag_server_bit ^= SNAPFLAG_SERVERCOUNT;
    //

    cvar_set("nextmap", "map_restart");

    // something_hunkusage(va("maps/mp/%s.bsp", server));
    
    cvar_set("cl_paused", "0");
    cm_load_map(va("maps/mp/%s.bsp", server), false, &checksum);
    cvar_set("mapname", server);

    // sv_serverid stuff
    
    sv.server_id = com_frame_time;
    sv.state = SS_LOADING;
    //

    cvar_set("sv_serverrestarting", "1");

    // load_soundaliases(va("maps/mp/%s.bsp", server), 2);
    // FUN_080cb8ee(1); just sets a variable to 1

    sv_init_game_progs(keep_persistent);

    // run a few frames to allow everything to settle
    for (i = 0; i < 3; i++) {
        svs.time += 100;
        #ifdef CODVERSION1_5
        svs.time2 += 100;
        #endif

        sv_game_run_frame();
    }

    sv_create_baseline();
    // update clients here

    strncpyz(system_info, cvar_info_string_big(CVAR_SYSTEM_INFO), 
               sizeof(system_info));
    cvar_modified_flags &= ~CVAR_SYSTEM_INFO;
    sv_set_config_string(CS_SYSTEM_INFO, system_info);

    sv_set_config_string(CS_SERVER_INFO, cvar_info_string(CVAR_2004));
    cvar_modified_flags &= ~CVAR_2004;

    sv_set_cvar_config_string(140, 64, 2048);
    cvar_modified_flags &= ~CVAR_WOLF_INFO;

    sv.state = SS_GAME;

    sv_heartbeat_f();

    cvar_set("sv_serverrestarting", "0");

    com_print_header("", 40, '-');
}

void INCOMPLETE sv_shutdown(UNUSED char *finalmsg)
{
    if (!com_sv_running || !com_sv_running->integer)
        return;

    com_print_header("server shutdown", 40, '-');

    // if (svs.clients && !com_error_entered())
    //	sv_final_message(finalmsg);

    // this needs to be called _only_ when com_shutdown happens
    // otherwise it will brick the server
    // sv_remove_operator_commands();

    // sv_master_shutdown();
    sv_shutdown_game_progs();

    // free the current level
    sv_clear_server();

    // if (svs.clients)
    //	sv_free_clients();

    // sv_free_script_stuff();

    // memset(&svs, 0, sizeof(svs));

    cvar_set("sv_running", "0");

    com_print_header("", 40, '-');

    // disconnect any local clients
    cl_disconnect(false);
}

void sv_shutdown_localized(char *localizedmsg)
{
    cl_disconnect(true);

    // blank function
    // fun_080846fe();

    sv_shutdown(localizedmsg);

    hunk_clear2();

    cl_start_hunk_users();
}
