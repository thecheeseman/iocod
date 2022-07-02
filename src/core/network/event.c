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

#include "net_local.h"

void net_event(fd_set *fdr)
{
    byte data[MAX_MSG_LEN + 1] = { 0 };
    struct netadr from = { 0 };
    struct netmsg msg = { 0 };

    while (true) {
        msg_init(&msg, data, sizeof(data));

        if (!net_get_packet(&from, fdr, &msg))
            break;

        // dropsim 

        #if 0
        if (com_sv_running->integer > 0)
            com_run_and_time_server_packet(&from, &msg);
        else
            cl_packet_event(from, &msg);
        #endif
    }
}
