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

#include "ev_local.h"

struct sys_event event_queue[MAX_QUEUED_EVENTS];
int event_head = 0;
int event_tail = 0;
byte packet_received[MAX_MSG_LEN];

struct sys_event pushed_events[MAX_PUSHED_EVENTS];
int pushed_events_head = 0;
int pushed_events_tail = 0;

IC_PUBLIC
void ev_init(void)
{
    memset(event_queue, 0, sizeof(event_queue));
    memset(packet_received, 0, sizeof(packet_received));
    memset(pushed_events, 0, sizeof(pushed_events));
}
