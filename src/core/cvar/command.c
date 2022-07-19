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
qbool cv_command(void)
{
    struct cvar *cv = cv_find(cmd_argv(0));
    if (cv == NULL)
        return false;

    // print variable
    if (cmd_argc() == 1) {
        cv_print(cv);
        return true;
    }

    // set value
    cv_set2(cv->name, cmd_args(), false);
    return true;
}
