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

IC_NON_NULL(1, 2)
void add_game_directory(_In_z_ const char *path,
                        _In_z_ const char *dir)
{
    char newdir[MAX_OSPATH] = { 0 };
    strncpyz(newdir, dir, sizeof(newdir));
    strncpyz(fs_gamedir, newdir, sizeof(fs_gamedir));

    searchpath_t *sp = (searchpath_t *) ic_malloc(sizeof(searchpath_t));
    sp->next = NULL;
    sp->pack = NULL;
    sp->dir = (directory_t *) ic_calloc(sizeof(directory_t), 1);
    strncpyz(sp->dir->path, path, sizeof(sp->dir->path));
    strncpyz(sp->dir->game, newdir, sizeof(sp->dir->game));

    add_search_path(sp);
    find_pack_files(path, newdir);
}
