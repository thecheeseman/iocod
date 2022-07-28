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

cvar_t *cl_language;

char last_valid_base[MAX_OSPATH];
char last_valid_game[MAX_OSPATH];

IC_PUBLIC
void fs_init(void)
{
    ic_print_header("filesystem", 20, '-');
    
    com_startup_variables(8, "fs_cdpath", "fs_basepath", "fs_homepath", 
                          "fs_game", "fs_copyfiles", "fs_restrict", 
                          "fs_usewolf", "cl_language");

    // seh init language
    cl_language = cv_get("cl_language", "0", CV_LATCH | CV_ARCHIVE);

    fs_startup(BASEGAME);

    // seh init stringed
    // seh update language info

    fs_set_restrictions();
    #if 0
    if (fs_read_file(DEFAULTCFG, NULL) <= 0) {
        ic_fatal(_("Couldn't load %s. Make sure iocod is run from "
                   "the correct folder."), DEFAULTCFG);
    }
    #endif

    strncpyz(last_valid_base, fs_basepath->string, sizeof(last_valid_base));
    strncpyz(last_valid_game, fs_gamedirvar->string, sizeof(last_valid_game));

    ic_print_header("", 20, '-');
}

static void shutdown_searchpaths(void)
{
    searchpath_t *next;
    for (searchpath_t *sp = fs_searchpaths; sp != NULL; sp = next) {
        if (sp->pack != NULL) {
            mz_zip_reader_end(sp->pack->handle);
            ic_free(sp->pack->handle);
            ic_free(sp->pack->build_buffer);
            ic_free(sp->pack);
        }

        if (sp->dir != NULL)
            ic_free(sp->dir);

        next = sp->next;
        ic_free(sp);
    }

    fs_searchpaths = NULL;
}

void fs_shutdown(qbool a)
{
    UNUSED_PARAM(a);
    
    // close open files

    // shutdown search paths
    shutdown_searchpaths();

    // shutdown file lists

    // remove commands
    fs_remove_commands();
}
