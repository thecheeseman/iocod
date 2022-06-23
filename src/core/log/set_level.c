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

/* string representation of log level */
static const char *level_str[] = {
    [LOG_LEVEL_NONE] = "none",
    [LOG_LEVEL_FATAL] = "fatal",
    [LOG_LEVEL_ERROR] = "error",
    [LOG_LEVEL_WARN] = "warn",
    [LOG_LEVEL_INFO] = "info",
    [LOG_LEVEL_DEBUG] = "debug",
    [LOG_LEVEL_TRACE] = "trace",
    [LOG_LEVEL_ALL] = "all"
};

/*
 * Set log level.
 */
IC_PUBLIC
void log_set_level(enum log_level new_level)
{
    IC_ASSERT((new_level >= LOG_LEVEL_NONE) && (new_level <= LOG_LEVEL_ALL));

    if (iclog.level != new_level) {
        log_debug(_("Log level changed from '%1$s' to '%2$s'\n"),
                  level_str[iclog.level], level_str[new_level]);

        iclog.level = new_level;
    }
}
