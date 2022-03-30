/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

/**
 * @file files.h
 * @date 2022-02-04
*/

#ifndef __FILES_H__
#define __FILES_H__

#define MAX_ZPATH           256
#define MAX_SEARCH_PATHS    4096
#define MAX_FILEHASH_SIZE   1024

// referenced flags
// these are in loop specific order so don't change the order
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

//
//
// files/files.c
// 
// 
bool fs_initialized(void);
void fs_clear_loadstack(void);
int fs_loadstack(void);
void fs_force_flush(filehandle f);
void fs_build_ospath(const char *base, const char *game, const char *qpath,
                     char *buf);
void fs_init_filesystem(void);
void fs_restart(int checksumfeed);
void fs_shutdown(bool closemfp);
void fs_pure_server_set_loaded_paks(const char *paksums, const char *paknames);
void fs_clear_pak_references(int flags);

//
//
// files/files_commands.c
//
//
void fs_add_commands(void);
void fs_remove_commands(void);

//
//
// files/files_io.c
//
//
void fs_fclose_file(filehandle f);
filehandle fs_fopen_file_write(const char *filename);
void fs_copy_file(char *fromospath, char *toospath);
void fs_remove(const char *ospath);
bool fs_file_exists(const char *file);
void fs_rename(const char *from, const char *to);
filehandle fs_fopen_file_append(const char *filename);
int fs_fopen_file_read(const char *filename, filehandle *file, bool uniquefile,
                       int a);
int fs_write(const void *buffer, int len, filehandle h);
void fs_printf(filehandle h, const char *fmt, ...);
int fs_read(void *buffer, int len, filehandle f);
int fs_read_file(const char *qpath, void **buffer);
void fs_free_file(void *buf);

#endif // __FILES_H__
