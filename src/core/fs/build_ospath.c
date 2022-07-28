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

IC_NON_NULL(1, 3, 4)
void fs_build_ospath(_In_z_ const char *base, 
                     _In_opt_z_ const char *game, 
                     _In_z_ const char *path,
                     _Out_ char *out, 
                     int a)
{
    if (game == NULL || *game == '\0')
        game = fs_gamedir;

    size_t baselen = strlen(base);
    size_t gamelen = strlen(game);
    size_t pathlen = strlen(path);

    if (baselen + gamelen + pathlen + 2 > MAX_OSPATH) {
        if (a != 0) {
            *out = '\0';
            return;
        }

        ic_fatal(_("OS Path length exceeded\n"));
    }

    memcpy(out, base, baselen);
    out[baselen] = '/';
    memcpy(out + baselen + 1, game, gamelen);
    out[baselen + gamelen + 1] = '/';
    memcpy(out + baselen + gamelen + 2, path, pathlen + 1);
    replace_separators(out);
}
