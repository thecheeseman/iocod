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

void net_netadr_to_sockaddr(struct netadr *a, struct sockaddr *s)
{
    if (a->type == NA_BROADCAST) {
        ((struct sockaddr_in *) s)->sin_family = AF_INET;
        ((struct sockaddr_in *) s)->sin_port = a->port;
        ((struct sockaddr_in *) s)->sin_addr.s_addr = INADDR_BROADCAST;
    } else if (a->type == NA_IP) {
        ((struct sockaddr_in *) s)->sin_family = AF_INET;
        ((struct sockaddr_in *) s)->sin_addr.s_addr = *(int *) &a->ip;
        ((struct sockaddr_in *) s)->sin_port = a->port;
    } else if (a->type == NA_IP6) {
        ((struct sockaddr_in6 *) s)->sin6_family = AF_INET6;
        ((struct sockaddr_in6 *) s)->sin6_addr = *((struct in6_addr *) &a->ip6);
        ((struct sockaddr_in6 *) s)->sin6_port = a->port;
        ((struct sockaddr_in6 *) s)->sin6_scope_id = a->scope_id;
    }
}

void net_sockaddr_to_netadr(struct sockaddr *s, struct netadr *a)
{
    if (s->sa_family == AF_INET) {
        a->type = NA_IP;

        *(int *) &a->ip = ((struct sockaddr_in *) s)->sin_addr.s_addr;

        a->port = ((struct sockaddr_in *) s)->sin_port;
    } else if (s->sa_family == AF_INET6) {
        a->type = NA_IP6;

        memcpy(a->ip6, &((struct sockaddr_in6 *) s)->sin6_addr, sizeof(a->ip6));

        a->port = ((struct sockaddr_in6 *) s)->sin6_port;
        a->scope_id = ((struct sockaddr_in6 *) s)->sin6_scope_id;
    }
}
