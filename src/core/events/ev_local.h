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

#ifndef EV_LOCAL_H
#define EV_LOCAL_H

#include "iocod.h"

#define MAX_QUEUED_EVENTS 256
#define MASK_QUEUED_EVENTS (MAX_QUEUED_EVENTS - 1)

#define MAX_PUSHED_EVENTS 256
#define MASK_PUSHED_EVENTS (MAX_PUSHED_EVENTS - 1)

// ev_init.c
extern struct sys_event event_queue[MAX_QUEUED_EVENTS];
extern int event_head;
extern int event_tail;
extern byte packet_received[MAX_MSG_LEN];

extern struct sys_event pushed_events[MAX_PUSHED_EVENTS];
extern int pushed_events_head;
extern int pushed_events_tail;

#endif /* EV_LOCAL_H */
