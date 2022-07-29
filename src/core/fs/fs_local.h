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

#define FS_GENERAL_REF  0x01
#define FS_UI_REF       0x02
#define FS_CGAME_REF    0x04
#define FS_QAGAME_REF   0x08

// these are identical to q3
// qagame_mp_x86.dll
#define SYS_DLLNAME_QAGAME_SHIFT 6
#define SYS_DLLNAME_QAGAME "wgmgskesve~><4jrr"

// cgame_mp_x86.dll
#define SYS_DLLNAME_CGAME_SHIFT 2
#define SYS_DLLNAME_CGAME "eicogaoraz:80fnn"

// ui_mp_x86.dll
#define SYS_DLLNAME_UI_SHIFT 5
#define SYS_DLLNAME_UI "zndrud}=;3iqq"

typedef struct packed_file {
    char *name;

    u32 index;
    i64 offset;
    i64 size;
    i64 size_uncompressed;

    mz_zip_archive_file_stat stat;

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

    i32 referenced;

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

union qfile_gu {
    FILE *o;
    mz_zip_archive *z;
};

struct qfile_u {
    union qfile_gu file;
    qbool unique;
};

typedef struct {
    struct qfile_u handle;
    qbool handle_sync;

    u64 offset;
    u64 size;
    u64 zip_offset;
    qbool zipfile;
    qbool streamed;

    char name[MAX_OSPATH];
} filehandle_data_t;

#define MAX_FILE_HANDLES 1024

extern filehandle_data_t fs_handle[MAX_FILE_HANDLES];

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

IC_NON_NULL(1, 2)
FILE *fs_fopen(_In_z_ const char *path,
               _In_z_ const char *mode);

/**
 * @brief Get the next available filehandle.
 * @return filehandle or 0 if failed
*/
filehandle handle_for_file();

/**
 * @brief Get the FILE pointer for a given @p f filehandle.
 * @param[in] f filehandle
 * @return FILE pointer or NULL if failed
*/
FILE *file_for_handle(filehandle f);

IC_NON_NULL(1)
i64 fs_filep_length(_In_ FILE *handle);

/**
 * @brief Get the length of a given @p f filehandle.
 * @param[in] f filehandle
 * @return length of the file or -1 if failed
*/
i64 fs_file_length(filehandle f);

IC_NON_NULL(1, 2)
int filename_compare(_In_z_ const char *s1,
                     _In_z_ const char *s2);

IC_NON_NULL(1, 2)
qbool extension_match(_In_z_ const char *filename,
                      _In_z_ const char *ext,
                      size_t namelen);

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

void fs_display_path(qbool skip_localized);

IC_NON_NULL(1)
u32 hash_filename(_In_z_ const char *filename,
                  u32 hash_size);

/**
 * @brief Determine if we should use a given searchpath. Always allows
 * non-localized searchpaths. For localized searchpaths, only match if we're
 * the current language and cvar `fs_ignoreLocalized` is set to 0.
 * 
 * @param[in] sp searchpath to check
 * @return true if we should use this searchpath, false otherwise
*/
IC_NON_NULL(1)
qbool use_localized_searchpath(_In_ searchpath_t *sp);

/**
 * @brief Open a file for reading.
 *
 * @param[in]  filename the name of the file to open
 * @param[in]  unique
 * @param[out] file     filehandle, or NULL to check if the file exists only
 * @return size of file, or if @p NULL, true or false if the file exists
*/
IC_PUBLIC
IC_NON_NULL(1)
i64 fs_fopen_file_read(_In_z_ const char *filename,
                       qbool unique,
                       _Out_opt_ filehandle *file);

#endif /* FS_LOCAL_H */
