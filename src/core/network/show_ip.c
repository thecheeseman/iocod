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
void net_show_ip(void)
{
    for (int i = 0; i < numip; i++) {
        char s[MAX_ADDR_STRING] = { 0 };
        net_sockaddr_to_string((struct sockaddr *) &localip[i].addr, 
                               sizeof(s), s);

        if (localip[i].type == NA_IP)
            log_print("IPv4: %s\n", s);
        else if (localip[i].type == NA_IP6)
            log_print("IPv6: %s\n", s);
    }
}
