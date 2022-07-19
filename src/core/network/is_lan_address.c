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
qbool net_is_lan_address(struct netadr addr)
{
    if (addr.type == NA_LOOPBACK)
        return true;

    if (addr.type == NA_IP) {
        // RFC1918:
        // 10.0.0.0        -   10.255.255.255  (10/8 prefix)
        // 172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
        // 192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
        if (addr.ip[0] == 10)
            return true;
        else if (addr.ip[0] == 172 && (addr.ip[1] & 0xf0) == 16)
            return true;
        else if (addr.ip[0] == 192 && addr.ip[1] == 168)
            return true;
        else if (addr.ip[0] == 127)
            return true;
    } else if (addr.type == NA_IP6) {
        if (addr.ip6[0] == 0xfe && (addr.ip6[1] & 0xc0) == 0x80)
            return true;
        if ((addr.ip6[0] & 0xfe) == 0xfc)
            return true;
    }

    // compare against the networks this computer is member of
    for (int i = 0; i < numip; i++) {
        byte *compare_adr = NULL;
        byte *compare_mask = NULL;
        byte *compare_ip = NULL;
        int addrsize = 0;

        if (localip[i].type == addr.type) {
            if (addr.type == NA_IP) {
                compare_ip = (byte *)
                    &((struct sockaddr_in *) &localip[i].addr)->sin_addr.s_addr;
                compare_mask = (byte *)
                    &((struct sockaddr_in *) &localip[i].netmask)->sin_addr.s_addr;
                compare_adr = addr.ip;

                addrsize = sizeof(addr.ip);
            } else if (addr.type == NA_IP6) {
                // TODO? should we check the scope_id here?

                compare_ip = (byte *)
                    &((struct sockaddr_in6 *) &localip[i].addr)->sin6_addr;
                compare_mask = (byte *)
                    &((struct sockaddr_in6 *) &localip[i].netmask)->sin6_addr;
                compare_adr = addr.ip6;

                addrsize = sizeof(addr.ip6);
            }

            qbool differed = false;
            for (int run = 0; run < addrsize; run++) {
                if ((compare_ip[run] & compare_mask[run]) !=
                    (compare_adr[run] & compare_mask[run])) {
                    differed = true;
                    break;
                }
            }

            if (!differed)
                return true;
        }
    }

    return false;
}
