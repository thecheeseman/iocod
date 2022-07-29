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

void fs_display_path(qbool skip_localized)
{
    const char *language = lz_get_language_name(lz_current_language());
    ic_printf(_("Current language: %s\n"), language);
    
    if (fs_ignore_localized->integer != 0)
        ic_printf(_("Localized assets are being ignored\n"));

    ic_printf(_("Current search path:\n"));
    for (searchpath_t *s = fs_searchpaths; s != NULL; s = s->next) {
        // skip alternative localized packs if they're not our 
        // current language
        if (skip_localized && !use_localized_searchpath(s))
            continue;

        if (s->pack != NULL) {
            ic_printf("%s (%d files)\n", s->pack->filename, s->pack->num_files);

            if (s->localized) {
                const char *packlang = lz_get_language_name(s->language);
                ic_printf(_("    localized assets pak file for %s\n"), packlang);
            }
        } else {
            ic_printf("%s%c%s\n", s->dir->path, PATH_SEP, s->dir->game);
        }
    }
}

static void path_f(void)
{
    fs_display_path(true);
}

static void fullpath_f(void)
{
    fs_display_path(false);
}

static void dir_f(void)
{

}

static void fdir_f(void)
{
    
}

static void touchfile_f(void)
{
    
}

void fs_add_commands(void)
{
    cmd_add("path", path_f);
    cmd_add("fullpath", fullpath_f);
    cmd_add("dir", dir_f);
    cmd_add("fdir", fdir_f);
    cmd_add("touchfile", touchfile_f);
}

void fs_remove_commands(void)
{
    cmd_remove("path");
    cmd_remove("fullpath");
    cmd_remove("dir");
    cmd_remove("fdir");
    cmd_remove("touchfile");
}
