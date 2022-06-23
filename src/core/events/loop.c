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

IC_PUBLIC
int ev_loop(void)
{
    // byte data[MAX_MSG_LEN];
    // msg_init(&buf, data, sizeof(data));

    while (true) {
        struct sys_event ev = ev_get();

        switch (ev.type) {
        case SE_KEY:
        case SE_CHAR:
        case SE_MOUSE:
        case SE_JOYSTICK_AXIS:
            break; // TODO: client
        case SE_CONSOLE:
            // cbuf_add_text((char *) ev.ptr);
            // cbuf_add_text("\n");
            break;
        case SE_PACKET:
            break; // TODO: net
        case SE_BAD_EVENT:
            log_error(_("Bad event type %i\n"), ev.type);
            break;
        default:
            // TODO: net loopback

            return ev.time;
        }

        ic_free(ev.ptr);
    }

    return 0;
}
