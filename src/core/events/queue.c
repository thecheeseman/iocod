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

IC_PUBLIC
void ev_queue(int time, enum sys_event_type type, int value, int value2,
              int ptr_length, void *ptr)
{
    struct sys_event *ev = &event_queue[event_head & MASK_QUEUED_EVENTS];

    if (event_head - event_tail >= MASK_QUEUED_EVENTS) {
        log_debug(_("Events overflow\n"));

        ic_free(ev->ptr);
        event_tail++;
    }

    event_head++;

    if (time == 0)
        time = sys_milliseconds();

    ev->time = time;
    ev->type = type;
    ev->value = value;
    ev->value2 = value2;
    ev->ptr_length = ptr_length;
    ev->ptr = ptr;
}
