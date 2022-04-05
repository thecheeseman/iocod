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

#include "common/cbuf.h"
#include "common/hunk.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "system/shared.h"
#include "server_local.h"
#include "stringlib.h"
#include "va.h"
#include "vm.h"

struct server sv;
struct vm *gvm = NULL;
struct server_static svs;

struct cvar *sv_gametype;
struct cvar *sv_mapname;
struct cvar *sv_privateclients;
struct cvar *sv_hostname;
struct cvar *sv_maxclients;
struct cvar *sv_maxrate;
struct cvar *sv_minping;
struct cvar *sv_maxping;
struct cvar *sv_floodprotect;
struct cvar *sv_allowanonymous;
struct cvar *sv_showcommands;
struct cvar *sv_serverid;
struct cvar *sv_pure;
struct cvar *sv_rconpassword;
struct cvar *sv_privatepassword;
struct cvar *sv_fps;
struct cvar *sv_timeout;
struct cvar *sv_zombietime;
struct cvar *sv_allowdownload;
struct cvar *sv_master[MAX_MASTER_SERVERS];
struct cvar *sv_reconnectlimit;
struct cvar *sv_showloss;
struct cvar *sv_padpackets;
struct cvar *sv_killserver;
struct cvar *sv_onlyvisibleclients;
struct cvar *sv_showaveragebps;
struct cvar *sv_maprotation;
struct cvar *sv_maprotation_current;

#ifdef CODVERSION1_5
struct cvar *sv_punkbuster;
struct cvar *sv_disableclientconsole;
struct cvar *sv_packet_info;
struct cvar *sv_kickbantime;
struct cvar *sv_wwwdownload;
struct cvar *sv_wwwbaseurl;
struct cvar *sv_wwwdldisconnected;
#endif

void INCOMPLETE sv_packet_event(UNUSED struct netadr from, UNUSED struct msg *msg)
{

}

bool sv_check_paused(void)
{
    int count, i;
    struct client *cl;

    if (!cl_paused->integer)
        return false;

    count = 0;
    for (i = 0, cl = svs.clients; i < sv_maxclients->integer; i++, cl++) {
        if (cl->state >= CS_CONNECTED && 
            cl->netchan.remote_address.type != NA_BOT)
            count++;
    }

    // can't pause if theres more than 1 player
    if (count > 1) {
        if (sv_paused->integer)
            cvar_set("sv_paused", "0");

        return false;
    }

    if (!sv_paused->integer)
        cvar_set("sv_paused", "1");

    return true;
}

void INCOMPLETE sv_set_cvar_config_string(UNUSED int index, UNUSED int count, UNUSED int bit)
{

}

void INCOMPLETE sv_calculate_pings(void)
{

}

// _DAT_080e30cc is only ever written to, never read from?
void sv_game_run_frame(void)
{
    vm_call(gvm, GAME_UPDATE_CVARS);

    // some magic here
    /*
    bVar1 = DAT_0833df04 == -1;
    DAT_0833df04 = DAT_0833df04 + 1;
    if (bVar1) {
        DAT_0833df04 = 1;
    }
    _DAT_080e30cc = 1;
    */
    // some magic here

    #ifdef CODVERSION1_5
    vm_call(gvm, GAME_RUN_FRAME, svs.time2);
    #else
    vm_call(gvm, GAME_RUN_FRAME, svs.time);
    #endif

    // _DAT_080e30cc = 0;
    hunk_swap_temp_low();
}

void INCOMPLETE sv_check_timeouts(void)
{

}

void INCOMPLETE sv_send_client_messages(void)
{

}

void INCOMPLETE sv_master_heartbeat(UNUSED const char *heartbeat)
{

}

void INCOMPLETE sv_bot_frame(UNUSED int time)
{

}

void INCOMPLETE sv_frame(int msec)
{
    int frame_msec;
    int start_time UNUSED;
    char mapname[64];

    if (sv_killserver->integer) {
        sv_shutdown_localized("EXE_SERVERKILLED");
        cvar_set("sv_killserver", "0");
        return;
    }

    if (!com_sv_running->integer)
        return;

    if (sv_check_paused())
        return;

    if (sv_fps->integer < 1)
        cvar_set("sv_fps", "10");
    frame_msec = 1000 / sv_fps->integer;

    sv.time_residual += msec;
    if (frame_msec > sv.time_residual)
        return;

    strncpyz(mapname, sv_mapname->string, sizeof(mapname));

    // if time is about to hit the 32nd bit, kick all clients
    // and clear sv.time, rather
    // than checking for negative time wraparound everywhere.
    // 2giga-milliseconds = 23 days, so it won't be too often
    #ifdef CODVERSION1_5
    if (svs.time > 0x70000000 || svs.time2 > 0x70000000) {
    #else
    if (svs.time > 0x70000000) {
    #endif
        sv_shutdown_localized("EXE_SERVERRESTARTTIMEWRAP");
        cbuf_add_text(va("map %s\n", mapname));
    }

    if (svs.next_snapshot_entities >= 0x7ffffffe - svs.num_snapshot_entities) {
        sv_shutdown_localized("EXE_SERVERRESTARTMISC\x15numSnapshotEntities");
        cbuf_add_text(va("map %s\n", mapname));
        return;
    }

    if (svs.next_cached_snapshot_entities >= 0x7fffbffe) {
        sv_shutdown_localized("EXE_SERVERRESTARTMISC\x15nextCachedSnapshotEntities");
        cbuf_add_text(va("map %s\n", mapname));
        return;
    }

    if (svs.next_cached_snapshot_clients >= 0x7fffeffe) {
        sv_shutdown_localized("EXE_SERVERRESTARTMISC\x15nextCachedSnapshotClients");
        cbuf_add_text(va("map %s\n", mapname));
        return;
    }

    if (svs.next_archived_snapshot_frames >= 0x7ffffb4e) {
        sv_shutdown_localized("EXE_SERVERRESTARTMISC\x15nextArchivedSnapshotFrames");
        cbuf_add_text(va("map %s\n", mapname));
        return;
    }

    if (svs.next_archived_snapshot_buffer >= 0x7dfffffe) {
        sv_shutdown_localized("EXE_SERVERRESTARTMISC\x15nextArchivedSnapshotBuffer");
        cbuf_add_text(va("map %s\n", mapname));
        return;
    }

    if (svs.next_cached_snapshot_frames >= 0x7ffffdfe) {
        sv_shutdown_localized("EXE_SERVERRESTARTMISC\x15nextCachedSnapshotFrames");
        cbuf_add_text(va("map %s\n", mapname));
        return;
    }

    if (svs.next_snapshot_clients >= 0x7ffffffe - svs.num_snapshot_clients) {
        sv_shutdown_localized("EXE_SERVERRESTARTMISC\x15numSnapshotClients");
        cbuf_add_text(va("map %s\n", mapname));
        return;
    }

    // FUN_080bfea0(1);

    if ((cvar_modified_flags & CVAR_2004)) {
        sv_set_config_string(CS_SERVER_INFO, cvar_info_string(CVAR_2004));
        cvar_modified_flags &= ~CVAR_2004;
    }

    if ((cvar_modified_flags & CVAR_SYSTEM_INFO)) {
        sv_set_config_string(CS_SYSTEM_INFO, 
                             cvar_info_string_big(CVAR_SYSTEM_INFO));
        cvar_modified_flags &= ~CVAR_SYSTEM_INFO;
    }

    if ((cvar_modified_flags & CVAR_WOLF_INFO)) {
        sv_set_cvar_config_string(140, 64, 2048);
        cvar_modified_flags &= ~CVAR_WOLF_INFO;
    }

    // fun_0809421b();

    if (com_speeds->integer)
        start_time = sys_milliseconds();
    else
        start_time = 0;

    sv_calculate_pings();

    // added
    if (com_dedicated->integer)
        sv_bot_frame(svs.time);
    // added

    while (true) {
        sv.time_residual -= frame_msec;
        svs.time += frame_msec;

        #ifdef CODVERSION1_5
        svs.time2 += frame_msec;
        #endif

        sv_game_run_frame();
        scr_set_loading(false);

        if (frame_msec < 1 || sv.time_residual < frame_msec)
            break;

        //sv_archival();
    }

    //if (com_speeds->integer)
    //		time_game = sys_milliseconds() - start_time;

    sv_check_timeouts();
    sv_send_client_messages();

    // if (com_timescale->value < 0.0)
    //		sv_archival();

    sv_master_heartbeat(HEARTBEAT_GAME);
}

char *sv_expand_new_lines(char *in) {
    static char string[MAX_STRING_CHARS];
    size_t l;

    l = 0;
    while (*in && l < sizeof(string) - 3) {
        if (*in == '\n') {
            string[l++] = '\\';
            string[l++] = 'n';
        } else {
            string[l++] = *in;
        }
        in++;
    }
    string[l] = 0;

    return string;
}

void INCOMPLETE sv_add_server_command(UNUSED struct client *cl, UNUSED const char *cmd)
{

}

// 1.5 has 4 params
void sv_send_server_command(struct client *cl, const char *fmt, ...)
{
    va_list argptr;
    char message[MAX_MSGLEN];
    struct client *client;
    int i;

    va_start(argptr, fmt);
    vsnprintf(message, sizeof(message), fmt, argptr);
    va_end(argptr);

    if (cl != NULL) {
        sv_add_server_command(cl, message);
        return;
    }

    // hack to echo broadcast prints to console
    if (com_dedicated->integer && !strncmp(message, "print", 5))
        com_printf("broadcast: %s\n", sv_expand_new_lines(message));

    // send the data to all relevant clients
    for (i = 0, client = svs.clients; i < sv_maxclients->integer; 
         i++, client++) {
        if (client->state < CS_PRIMED)
            continue;

        sv_add_server_command(client, message);
    }
}

void INCOMPLETE svc_get_status(UNUSED struct netadr from)
{

}

void INCOMPLETE svc_get_info(UNUSED struct netadr from)
{

}

void INCOMPLETE sv_get_challenge(UNUSED struct netadr from)
{

}

void INCOMPLETE sv_direct_connect(UNUSED struct netadr from)
{

}

void INCOMPLETE sv_authorize_ip_packet(UNUSED struct netadr from)
{

}

void INCOMPLETE sv_remote_command(UNUSED struct netadr from)
{

}

void INCOMPLETE sv_connectionless_packet(struct netadr from, struct msg *msg)
{
    char *s, *c;

    msg_begin_reading_oob(msg);
    msg_read_long(msg); // skip the -1 marker

    // some netchan allocation here
    // fun_0808dd10(msg->cursize);

    #ifdef CODVERSION1_5
    // punkbuster stuff
    if (strncasecmp(&msg->data[4], "pb_", 3) == 0)
        return;
    #endif

    if (strncmp("connect", (const char *) & msg->data[4], 7) == 0)
        huff_decompress(msg, 12);

    s = msg_read_string_line(msg);
    cmd_tokenize_string(s);

    c = cmd_argv(0);

    com_dprintf("sv packet %s: %s\n", net_address_to_string(from), c);

    if (strcasecmp(c, "getstatus") == 0)
        svc_get_status(from);
    else if (strcasecmp(c, "getinfo") == 0)
        svc_get_info(from);
    else if (strcasecmp(c, "getchallenge") == 0)
        sv_get_challenge(from);
    else if (strcasecmp(c, "connect") == 0)
        sv_direct_connect(from);
    else if (strcasecmp(c, "ipauthorize") == 0)
        sv_authorize_ip_packet(from);
    else if (strcasecmp(c, "rcon") == 0)
        sv_remote_command(from);
    //else if (qstricmp(c, "disconnect") == 0)
    else
        com_dprintf("bad connectionless packet from %s:\n%s\n", 
                    net_address_to_string(from), s);
}
