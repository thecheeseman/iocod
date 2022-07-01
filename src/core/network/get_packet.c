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

bool net_get_packet(struct netadr *net_from, struct netmsg *msg, fd_set *fdr)
{
    for (int protocol = 0; protocol < 2; protocol++) {
        socket_t sock = INVALID_SOCKET;
        if (protocol == 0)
            sock = ip_socket;
        else if (protocol == 1)
            sock = ip6_socket;

        if (sock != INVALID_SOCKET && FD_ISSET(sock, fdr)) {
            struct sockaddr_storage from = { 0 };
            socklen_t fromlen = sizeof(from);

            int ret = recvfrom(sock, (void *) msg->data, msg->max_size, 0,
                               (struct sockaddr *) &from, &fromlen);

            if (ret == SOCKET_ERROR) {
                int err = sockerror();
                if (err == EAGAIN || err != ECONNRESET)
                    log_print("%s", net_error_string());

                continue;
            }

            net_sockaddr_to_netadr((struct sockaddr *) &from, net_from);
            msg->read_count = 0;

            if (ret >= msg->max_size) {
                log_print(_("Oversize packet from '%s'"),
                          net_address_to_string(*net_from));
                return false;
            }

            msg->cur_size = ret;
            return true;
        }
    }

    return false;
}
