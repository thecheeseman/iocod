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

cvar_t *fs_debug;
cvar_t *fs_copyfiles;
cvar_t *fs_basepath;
cvar_t *fs_basegame;
cvar_t *fs_homepath;
cvar_t *fs_gamedirvar;
cvar_t *fs_restrict;
cvar_t *fs_ignore_localized;

searchpath_t *fs_searchpaths = NULL;
extern u32 fs_packed_files = 0;

char fs_gamedir[MAX_OSPATH];

#define add_file_directory(x, b)

void fs_startup(const char *basegame)
{
    fs_debug = cv_get("fs_debug", "0", CV_NONE);
    fs_copyfiles = cv_get("fs_copyfiles", "0", CV_INIT);
    fs_basepath = cv_get("fs_basepath", sys_cwd(), CV_INIT);
    fs_basegame = cv_get("fs_basegame", "", CV_INIT);

    char *homepath = sys_default_homepath();
    if (homepath == NULL || *homepath == '\0')
        homepath = fs_basepath->string;
    fs_homepath = cv_get("fs_homepath", homepath, CV_INIT);
    fs_gamedirvar = cv_get("fs_game", "", CV_INIT | CV_SYSTEM_INFO);
    fs_restrict = cv_get("fs_restrict", "", CV_INIT);
    fs_ignore_localized = cv_get("fs_ignoreLocalized", "0", CV_LATCH | CV_CHEAT);

    // add basepath game directories
    if (*fs_basepath->string != '\0') {
        add_game_directory(fs_basepath->string, basegame);

        if (strcasecmp(fs_homepath->string, fs_basepath->string) != 0)
            add_game_directory(fs_homepath->string, basegame);
    }

    // add any other basegame mods
    if (*fs_basegame->string != '\0') {
        if (strcasecmp(basegame, BASEGAME) == 0 &&
            strcasecmp(fs_basegame->string, basegame) != 0) {
            
            if (*fs_basepath->string != '\0')
                add_game_directory(fs_basepath->string, fs_basegame->string);
            
            if (*fs_homepath->string != '\0') {
                if (strcasecmp(fs_homepath->string, fs_basepath->string) != 0)
                    add_game_directory(fs_homepath->string, fs_basegame->string);
            }
        }
    }

    // other mods
    if (*fs_gamedirvar->string != '\0') {
        if (strcasecmp(basegame, BASEGAME) == 0 &&
            strcasecmp(fs_gamedirvar->string, basegame) != 0) {

            if (*fs_basepath->string != '\0')
                add_game_directory(fs_basepath->string, fs_gamedirvar->string);

            if (*fs_homepath->string != '\0') {
                if (strcasecmp(fs_homepath->string, fs_basepath->string) != 0)
                    add_game_directory(fs_homepath->string, fs_gamedirvar->string);
            }
        }
    }
    
    // directories (not in pk3 files) to read
    // e.g. main/xmodel
    add_file_directory("xanim", "");
    add_file_directory("xmodel", "");
    add_file_directory("xmodelparts", "");
    add_file_directory("xmodelsurfs", "");
    add_file_directory("weapons", "");
    add_file_directory("animtrees", ".atr");

    // cd key stuff -- unnecessary?

    // fs commands
    fs_add_commands();

    // display path information to console
    fs_display_path(true);

    // total # pak files
    ic_printf("%d files in pk3 files\n", fs_packed_files);
}
