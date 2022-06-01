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

#include "ev_local.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/*
 * Get system event 
 */
static struct sys_event get_event(void)
{
    // return if we have event data
    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(event_tail - 1) & MASK_QUEUED_EVENTS];
    }

    #ifdef IC_PLATFORM_WINDOWS
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
        if (!GetMessage(&msg, NULL, 0, 0))
            exit(1);

        // window_vars.msg_time = msg.time;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    #endif

    // TODO: console input

    // TODO: network

    // return if we have event data
    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(event_tail - 1) & MASK_QUEUED_EVENTS];
    }

    // otherwise, create an empty event to return
    struct sys_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.time = ic_milliseconds();

    return ev;
}

IC_PUBLIC
struct sys_event ev_get_real(void)
{
    // TODO: journaling

    struct sys_event ev = get_event();
    return ev;
}

IC_PUBLIC
struct sys_event ev_get(void)
{
    // return if we have an event already
    if (pushed_events_head > pushed_events_tail) {
        pushed_events_tail++;

        return pushed_events[(pushed_events_tail - 1) & MASK_PUSHED_EVENTS];
    }

    return ev_get_real();
}
