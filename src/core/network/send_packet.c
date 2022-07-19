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

IC_PUBLIC
qbool net_send_packet(int length, const void *data, struct netadr to)
{
    if (to.type != NA_BROADCAST && to.type != NA_IP && to.type != NA_IP6) {
        ic_error(_("bad address type"));
        return false;
    }

    if ((ip_socket == INVALID_SOCKET && to.type == NA_IP) ||
        (ip6_socket == INVALID_SOCKET && to.type == NA_IP6)) {
        log_debug(_("invalid socket"));
        return false;
    }

    struct sockaddr_storage addr = { 0 };
    memset(&addr, 0, sizeof(addr));
    net_netadr_to_sockaddr(&to, (struct sockaddr *) &addr);

    int ret = 0;
    if (addr.ss_family == AF_INET) {
        ret = sendto(ip_socket, data, length, 0, (struct sockaddr *) &addr,
                     sizeof(struct sockaddr_in));
    } else if (addr.ss_family == AF_INET6) {
        ret = sendto(ip_socket, data, length, 0, (struct sockaddr *) &addr,
                     sizeof(struct sockaddr_in6));
    }

    if (ret == SOCKET_ERROR) {
        #ifdef IC_PLATFORM_WINDOWS
        int err = WSAGetLastError();
        #else
        int err = errno;
        #endif

        // wouldblock is silent
        if (err == EAGAIN)
            return false;

        log_debug(_("sendto failed: %s"), net_error_string());
        return false;
    }

    return true;
}
