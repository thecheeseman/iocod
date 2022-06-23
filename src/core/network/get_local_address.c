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

struct localaddr localip[MAX_IPS];
int numip;

static void add_local_address(char *ifname, struct sockaddr *addr,
                              struct sockaddr *netmask)
{
    if (ifname == NULL || addr == NULL || netmask == NULL)
        return;

    sa_family_t family = addr->sa_family;
    int addrlen;

    if (numip < MAX_IPS) {
        struct localaddr *ip = &localip[numip];

        if (family == AF_INET) {
            addrlen = sizeof(struct sockaddr_in);
            ip->type = NA_IP;
        } else if (family == AF_INET6) {
            addrlen = sizeof(struct sockaddr_in6);
            ip->type = NA_IP6;
        } else {
            return; // bad trype
        }

        strncpyz(ip->ifname, ifname, sizeof(ip->ifname));
        ip->family = family;

        memcpy(&ip->addr, addr, addrlen);
        memcpy(&ip->netmask, netmask, addrlen);

        numip++;
    }
}

void net_get_local_address(void)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        log_warn("gethostname: %s\n", net_error_string());
        return;
    }

    log_print(_("Hostname: %s"), hostname);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *result = NULL;
    if (getaddrinfo(hostname, "0", &hints, &result) != 0) {
        log_warn("getaddrinfo: %s\n", net_error_string());
        freeaddrinfo(result);
        return;
    }

    struct sockaddr_in mask4;
    struct sockaddr_in6 mask6;

    memset(&mask4, 0, sizeof(mask4));
    memset(&mask4.sin_addr.s_addr, 0xFF, sizeof(mask4.sin_addr.s_addr));
    memset(&mask6, 0, sizeof(mask6));
    memset(&mask6.sin6_addr, 0xFF, sizeof(mask6.sin6_addr));

    mask4.sin_family = AF_INET;
    mask6.sin6_family = AF_INET6;

    struct addrinfo *ptr = NULL;
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        if (ptr->ai_family == AF_INET)
            add_local_address("", ptr->ai_addr, (struct sockaddr *) &mask4);
        else if (ptr->ai_family == AF_INET6)
            add_local_address("", ptr->ai_addr, (struct sockaddr *) &mask6);
    }

    net_show_ip();

    freeaddrinfo(result);
}
