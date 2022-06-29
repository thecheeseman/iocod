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

#include "cvar_local.h"

IC_PUBLIC
struct cvar *cv_reset(const char *name)
{
    return cv_set2(name, NULL, false);
}

void cv_reset_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    if (cmd_argc() != 2) {
        ic_printf(_("usage: reset <variable>\n"));
        return;
    }

    cv_reset(cmd_argv(1));
}
