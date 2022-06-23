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

#include "log_local.h"

/*
 * Log clear.
 */
IC_PUBLIC
void log_clear(void)
{
    if (iclog.fp == NULL)
        return;

    iclog.fp = freopen(iclog.path, "wb", iclog.fp);
    if (iclog.fp == NULL) {
        ic_error(_("Couldn't open logfile '%1$s': %2$s\n"),
                 iclog.path, strerror(errno));
        return;
    }

    log_banner();
    log_debug(_("Log cleared\n"));
}
