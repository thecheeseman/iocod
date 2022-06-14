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
 * @file net_chan.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"
#include "common/memory.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "va.h"

struct cvar *showpackets;
struct cvar *showdrop;
struct cvar *qport;
struct cvar *profile;
struct cvar *showprofile;
struct cvar *lanauthorize;

int profiling_mode;

#define MAX_PACKETLEN 1400

#define MAX_LOOPBACK 16
struct loopmsg {
    byte data[MAX_PACKETLEN];
    int datalen;
};

struct loopback {
    struct loopmsg msgs[MAX_LOOPBACK];
    int get, send;
};

struct loopback loopbacks[2];

static void net_dumpprofile_f(void)
{
    if (profiling_mode == 0)
        com_printf("Network profiling is not on. " \
                   "Set net_profile to turn on network profiling\n");
    // else
    // fun_0808d4d0(1);
}

void netchan_init(int port)
{
    showpackets = cvar_get("showpackets", "0", CVAR_TEMP);
    showdrop = cvar_get("showdrop", "0", CVAR_TEMP);
    qport = cvar_get("net_qport", va("%i", port), CVAR_INIT);
    profile = cvar_get("net_profile", "0", CVAR_TEMP);
    showprofile = cvar_get("net_showprofile", "0", CVAR_TEMP);
    lanauthorize = cvar_get("net_lanauthorize", "0", 0);

    cmd_add_command("net_dumpprofile", net_dumpprofile_f);
}

bool net_get_loop_packet(int sock, struct netadr *from, struct msg *msg)
{
    int i;
    struct loopback *loop;

    loop = &loopbacks[sock];

    if (loop->send - loop->get > MAX_LOOPBACK)
        loop->get = loop->send - MAX_LOOPBACK;

    if (loop->get >= loop->send)
        return false;

    i = loop->get & (MAX_LOOPBACK - 1);
    loop->get++;

    com_memcpy(msg->data, loop->msgs[i].data, loop->msgs[i].datalen);
    msg->cursize = loop->msgs[i].datalen;
    com_memset(from, 0, sizeof(*from));
    from->type = NA_LOOPBACK;
    return true;
}
