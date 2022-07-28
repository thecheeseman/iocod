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

#include "fs_local.h"

void fs_set_restrictions(void)
{
    if (fs_restrict->integer == 0)
        return;

    //cv_set("fs_restrict", 1);
    cv_set_integer("fs_restrict", 1);

    ic_printf(_("\nRunning in restricted demo mode\n"));
    fs_shutdown(false);
    fs_startup(BASEGAME);

    // check pak0 checksum
}
