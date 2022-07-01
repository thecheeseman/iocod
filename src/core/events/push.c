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
void ev_push(struct sys_event *event)
{
    struct sys_event *ev =
        &pushed_events[pushed_events_head & (MASK_PUSHED_EVENTS)];

    static bool warned = false;
    if (pushed_events_head - pushed_events_tail >= MAX_PUSHED_EVENTS) {
        // avoid duplicate warnings
        if (!warned) {
            log_warn(_("Pushed events overflow\n"));
            warned = true;
        }

        ic_free(ev->ptr);

        pushed_events_tail++;
    } else {
        warned = false;
    }

    *ev = *event;
    pushed_events_head++;
}
