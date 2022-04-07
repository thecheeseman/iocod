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

#include <errno.h>

#include "shared.h"
#include "common.h"
#include "common/error.h"
#include "common/memory.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "strings/stringlib.h"
#include "iocod.h"
#include "va.h"

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
        if ((com_memcmp(a.ipx, b.ipx, 10) == 0))
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
        if ((com_memcmp(a.ipx, b.ipx, 10) == 0) && a.port == b.port)
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
        snprintfz(s, sizeof(s), "loopback");
    } else if (a.type == NA_BOT) {
        snprintfz(s, sizeof(s), "bot");
    } else if (a.type == NA_IP) {
        snprintfz(s, sizeof(s), 
                    "%i.%i.%i.%i:%hu",
                    a.ip[0], a.ip[1], a.ip[2], a.ip[3], bigshort(a.port));
    } else {
        snprintfz(s, sizeof(s), 
                    "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x:%hu",
                    a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4],
                    a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9],
                    bigshort(a.port));
    }

    return s;
}
