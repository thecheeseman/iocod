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

#include <errno.h>
#include "conf_local.h"

static int conferrno = CONF_ERR_OK;

static char *errstrs[] = {
    "OK",
    "NULL conf pointer",
    "NULL filename",
    "NULL options pointer",
    "Error while stat(): %s",
    "Error while fopen(): %s",
    "Error while fread(): %s",
    "Line %d is incomplete",
    "Token too large on line %d",
    "Expected value for token '%s'",
    "Unknown value for boolean: %s",
    "Unknown config variable type %d"
};

static char conferrmsg[1024];
void conf_set_error(int err, ...)
{
    conferrno = err;

    va_list argptr;
    va_start(argptr, err);
    vsnprintf(conferrmsg, sizeof(conferrmsg), errstrs[conferrno], argptr);
    va_end(argptr);
}

char *conf_error_string(void)
{
    return conferrmsg;
}
