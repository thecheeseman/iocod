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
#include "system/events.h"
#include "stringlib.h"

struct system_event INCOMPLETE sys_get_event(void)
{
    struct system_event ev;
    char *s = NULL;
    //struct msg netmsg;
    //struct netadr adr;

    // return if we have event data
    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(event_tail - 1) & MASK_QUEUED_EVENTS];
    }

    // sys_send_key_events();

    // check for console commands
    s = sys_console_input();
    if (s) {
        char *b;
        int len;

        len = strlen(s) + 1;
        b = z_malloc(len);
        strcpy(b, s);
        queue_event(0, SE_CONSOLE, 0, 0, len, b);
    }

    // in_frame();

    // check for network packets

    // return if we have event data
    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(eventtail - 1) & MASK_QUEUED_EVENTS];
    }

    // create an empty event to return
    com_memset(&ev, 0, sizeof(ev));
    ev.time = sys_milliseconds();

    return ev;
}
