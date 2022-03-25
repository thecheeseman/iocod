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

/**
 * @file net.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"
#include "common/error.h"
#include "common/print.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> // bk001204

#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <errno.h>

#define PORT_SERVER 28960

struct cvar *noudp;
int ip_socket;

#define MAX_IPS 16
static int numip;
static byte localip[MAX_IPS][4];

char *net_error_string(void)
{
    int code = errno;
    return strerror(code);
}

bool net_compare_base_address(struct netadr a, struct netadr b)
{
    if (a.type != b.type)
        return false;

    if (a.type == NA_LOOPBACK)
        return true;

    if (a.type == NA_IP) {
        if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && 
            a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3])
            return true;

        return false;
    }

    if (a.type == NA_IPX) {
        if ((memcmp(a.ipx, b.ipx, 10) == 0))
            return true;

        return false;
    }

    com_warning("bad address type\n");
    return false;
}

bool net_compare_address(struct netadr a, struct netadr b)
{
    if (a.type != b.type)
        return false;

    if (a.type == NA_LOOPBACK)
        return true;

    if (a.type == NA_IP) {
        if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && 
            a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] && a.port == b.port)
            return true;

        return false;
    }

    if (a.type == NA_IPX) {
        if ((memcmp(a.ipx, b.ipx, 10) == 0) && a.port == b.port)
            return true;

        return false;
    }

    com_warning("bad address type\n");
    return false;
}

const char *net_address_to_string(struct netadr a)
{
    static char s[64];

    if (a.type == NA_LOOPBACK) {
        com_sprintf(s, sizeof(s), "loopback");
    } else if (a.type == NA_BOT) {
        com_sprintf(s, sizeof(s), "bot");
    } else if (a.type == NA_IP) {
        com_sprintf(s, sizeof(s), 
                    "%i.%i.%i.%i:%hu",
                    a.ip[0], a.ip[1], a.ip[2], a.ip[3], bigshort(a.port));
    } else {
        com_sprintf(s, sizeof(s), 
                    "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x:%hu",
                    a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4],
                    a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9],
                    bigshort(a.port));
    }

    return s;
}

bool sys_string_to_sockaddr(const char *s, struct sockaddr *sadr)
{
    struct hostent *h;

    memset(sadr, 0, sizeof(*sadr));
    ((struct sockaddr_in *) sadr)->sin_family = AF_INET;

    ((struct sockaddr_in *) sadr)->sin_port = 0;

    if (s[0] >= '0' && s[0] <= '9') {
        *(intptr_t *) &((struct sockaddr_in *) sadr)->sin_addr = inet_addr(s);
    } else {
        if ((h = gethostbyname(s)) == NULL)
            return false;

        *(intptr_t *) &((struct sockaddr_in *) sadr)->sin_addr = 
            *(intptr_t *) h->h_addr_list[0];
    }

    return true;
}

void net_init(void)
{
    noudp = cvar_get("net_noudp", "0", 0);

    if (noudp->value == 0.0 && ip_socket == 0)
        net_open_ip();
}

void net_shutdown(void)
{
    if (ip_socket != 0) {
        close(ip_socket);
        ip_socket = 0;
    }
}

void net_get_local_address(void)
{
    char hostname[256], *p;
    struct hostent *hostinfo;
    int ip, n;

    if (gethostname(hostname, 256) == -1)
        return;

    hostinfo = gethostbyname(hostname);
    if (hostinfo == NULL)
        return;

    com_printf("hostname: %s\n", hostinfo->h_name);
    n = 0;
    while ((p = hostinfo->h_aliases[n++]) != NULL)
        com_printf("alias: %s\n", p);

    if (hostinfo->h_addrtype != AF_INET)
        return;

    numip = 0;
    while ((p = hostinfo->h_addr_list[numip++]) != NULL && numip < MAX_IPS) {
        ip = ntohl(*(int *) p);

        localip[numip][0] = p[0];
        localip[numip][1] = p[1];
        localip[numip][2] = p[2];
        localip[numip][3] = p[3];

        com_printf("IP: %i.%i.%i.%i\n", 
            (ip >> 24) & 0xff, 
            (ip >> 16) & 0xff,
            (ip >> 8) & 0xff,
            ip & 0xff);
    }
}

#define PORT_ANY -1

int net_ip_socket(char *netif, int port)
{
    int newsock, i = 1;
    struct sockaddr_in address;
    int fionbioval = 1;

    if (netif != NULL)
        com_printf("Opening IP socket: %s:%i\n", netif, port);
    else
        com_printf("Opening IP socket: localhost:%i\n", netif, port);

    if ((newsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        com_printf("ERROR: udp_opensocket: socket: %s", net_error_string());
        return 0;
    }

    // open a non-blocking socket
    if (ioctl(newsock, FIONBIO, &fionbioval) == -1) {
        com_printf("ERROR: udp_opensocket: ioctl FIONBIO: %s\n", 
                   net_error_string());
        return 0;
    }

    // broadcast capable
    if (setsockopt(newsock, SOL_SOCKET, SO_BROADCAST, 
                   (char *) &i, sizeof(i)) == -1) {
        com_printf("ERROR: udp_opensocket: setsockopt SO_BROADCAST: %s\n", 
                   net_error_string());
        return 0;
    }

    if (netif != NULL || *netif != '\0' || q_stricmp(netif, "localhost") == 0)
        address.sin_addr.s_addr = INADDR_ANY;
    else
        sys_string_to_sockaddr(netif, (struct sockaddr *) &address);

    if (port == PORT_ANY)
        address.sin_port = 0;
    else
        address.sin_port = htons((short) port);

    address.sin_family = AF_INET;

    if (bind(newsock, (void *) &address, sizeof(address)) == -1) {
        com_printf("ERROR: udp_opensocket: bind: %s\n", net_error_string());
        close(newsock);
        return 0;
    }

    return newsock;
}

void net_open_ip(void)
{
    struct cvar *ip;
    int port, i;

    ip = cvar_get("net_ip", "localhost", 0);
    port = cvar_get("net_port", va("%i", PORT_SERVER), 0)->value;

    for (i = 0; i < 10; i++) {
        ip_socket = net_ip_socket(ip->string, port + i);

        if (ip_socket != 0) {
            cvar_set_value("net_port", port + i);
            net_get_local_address();
            return;
        }
    }

    com_error(ERR_FATAL, "Couldn't allocate IP port");
}
