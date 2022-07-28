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

#ifndef FS_LOCAL_H
#define FS_LOCAL_H

#include "iocod.h"
#include "miniz/miniz.h"

typedef struct packed_file {
    char *name;

    u64 pos;
    u64 len;

    struct packed_file *next;
} packed_file_t;

typedef struct {
    char path[MAX_OSPATH];
    char filename[MAX_OSPATH];
    char basename[MAX_OSPATH];
    char gamename[MAX_OSPATH];

    mz_zip_archive *handle;

    i32 checksum;
    i32 pure_checksum;
    u32 num_files;
    u32 hash_size;

    packed_file_t **hash_table;
    packed_file_t *build_buffer;
} pack_t;

typedef struct {
    char path[MAX_OSPATH];
    char game[MAX_OSPATH];
} directory_t;

typedef struct searchpath {
    struct searchpath *next;

    pack_t *pack;
    directory_t *dir;

    qbool localized;
    u8 language;
} searchpath_t;

extern searchpath_t *fs_searchpaths;
extern u32 fs_packed_files;

extern char fs_gamedir[MAX_OSPATH];
extern char last_valid_base[MAX_OSPATH];
extern char last_valid_game[MAX_OSPATH];

extern cvar_t *fs_debug;
extern cvar_t *fs_copyfiles;
extern cvar_t *fs_basepath;
extern cvar_t *fs_basegame;
extern cvar_t *fs_homepath;
extern cvar_t *fs_gamedirvar;
extern cvar_t *fs_restrict;
extern cvar_t *fs_ignore_localized;

/**
 * @brief Replace path separator in @p path with matching OS preference. 
 * @param[in,out] path path to replace separator in
*/
void replace_separators(char *path);

void fs_startup(const char *basegame);

void fs_add_commands(void);
void fs_remove_commands(void);
void fs_set_restrictions(void);

IC_NON_NULL(1, 3, 4)
void fs_build_ospath(_In_z_ const char *base,
                     _In_opt_z_ const char *game,
                     _In_z_ const char *path,
                     _Out_ char *out,
                     int a);

IC_NON_NULL(1)
void add_search_path(_In_ searchpath_t *sp);

IC_NON_NULL(1, 2)
void add_game_directory(_In_z_ const char *path,
                        _In_z_ const char *dir);

IC_NON_NULL(1, 2)
void find_pack_files(_In_z_ const char *path, 
                     _In_z_ const char *newdir);

IC_NON_NULL(1)
pack_t *fs_load_zip(_In_z_ const char *zipfile);

void fs_display_path(qbool a);

#endif /* FS_LOCAL_H */
