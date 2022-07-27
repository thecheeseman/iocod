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
IC_RETURNS_STRING
char *net_address_to_string(struct netadr addr)
{
    static char s[MAX_ADDR_STRING] = { 0 };

    switch (addr.type) {
    case NA_LOOPBACK:
        snprintf(s, sizeof(s), "loopback");
        break;
    case NA_BOT:
        snprintf(s, sizeof(s), "bot");
        break;
    case NA_IP:
    case NA_IP6:
    {
        struct sockaddr_storage sadr;

        memset(&sadr, 0, sizeof(sadr));

        net_netadr_to_sockaddr(&addr, (struct sockaddr *) &sadr);
        net_sockaddr_to_string((struct sockaddr *) &sadr, sizeof(s), s);
        break;
    }
    case NA_BAD:
    default:
        snprintf(s, sizeof(s), "BADADDR");
        break;
    }

    return s;
}

IC_PUBLIC
IC_RETURNS_STRING
char *net_address_to_string_port(struct netadr addr)
{
    static char s[MAX_ADDR_STRING] = { 0 };

    switch (addr.type) {
    case NA_IP:
        snprintf(s, sizeof(s), "%s:%hu", net_address_to_string(addr),
                 addr.port);
        break;
    case NA_IP6:
        snprintf(s, sizeof(s), "[%s]:%hu", net_address_to_string(addr),
                 addr.port);
        break;
    default:
        snprintf(s, sizeof(s), "%s", net_address_to_string(addr));
        break;
    }

    return s;
}
