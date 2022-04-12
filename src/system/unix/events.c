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

#include "shared.h"
#include "common.h"
#include "common/memory.h"
#include "common/print.h"
#include "strings/stringlib.h"
#include "system/events.h"
#include "system/shared.h"

struct system_event get_event(void)
{
    struct system_event ev;
    char *s = NULL;
    struct msg netmsg;
    struct netadr adr;
    char *b;
    int len;

    // return if we have event data
    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(event_tail - 1) & MASK_QUEUED_EVENTS];
    }

    send_key_events();

    // check for console commands
    s = console_input();
    if (s) {
        len = strlen(s) + 1;
        b = z_malloc(len);
        strcpy(b, s);
        queue_event(0, SE_CONSOLE, 0, 0, len, b);
    }

    in_frame();

    // check for network packets
    msg_init(&netmsg, packet_received, sizeof(packet_received));
    if (get_packet(&adr, &netmsg)) {
        struct netadr *buf = z_malloc(len);

        len = sizeof(struct netadr) + netmsg.cursize;
        *buf = adr;
        memcpy(buf + 1, netmsg.data, netmsg.cursize);
        queue_event(0, SE_PACKET, 0, 0, len, buf);
    }

    // return if we have event data
    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(event_tail - 1) & MASK_QUEUED_EVENTS];
    }

    // create an empty event to return
    com_memset(&ev, 0, sizeof(ev));
    ev.time = sys_milliseconds();

    return ev;
}
