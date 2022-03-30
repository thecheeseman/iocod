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
 * @file files_local.h
 * @date 2022-02-04
*/

#ifndef __FILES_LOCAL_H__
#define __FILES_LOCAL_H__

#include "shared.h"
#include "common.h"
#include "unzip.h"

#include "common/print.h"
#include "cvar/cvar.h"

struct fileinpack {
    char *name;
	uint32_t pos;
	int a;
	struct fileinpack *next;
};

struct pack {
	char pak_filename[MAX_OSPATH];
	char pak_basename[MAX_OSPATH];
	char pak_gamename[MAX_OSPATH];
	unzFile handle;
	int checksum;
	int pure_checksum;
	int numfiles;
	int referenced;
	uint32_t hashsize;
	struct fileinpack **hashtable;
	struct fileinpack *buildbuffer;
};

struct directory {
	char path[MAX_OSPATH];
	char gamedir[MAX_OSPATH];
};

struct searchpath {
	struct searchpath *next;

	struct pack *pack;
	struct directory *dir;

	int localized;
	int b;
};

union qfile_gu {
	FILE *o;
	unzFile z;
};

struct qfile_u {
	union qfile_gu file;
	bool unique;
};

struct filehandledata {
	struct qfile_u handlefiles;
	bool handlesync;
	int baseoffset;
	int filesize;
	int zipfilepos;
	bool zipfile;
	bool streamed;
	char name[MAX_ZPATH];
};

extern char fs_gamedir[MAX_OSPATH];

extern int fs_readcount;
extern int _fs_loadstack;
extern int fs_packfiles;
extern int fs_fake_checksum;
extern int fs_checksumfeed;
extern struct searchpath *fs_searchpaths;
extern struct filehandledata fsh[MAX_FILE_HANDLES];

extern int fs_num_serverpaks;
extern int fs_serverpaks[MAX_SEARCH_PATHS]; // checksums
extern char *fs_server_paknames[MAX_SEARCH_PATHS]; // pk3 names

extern int fs_num_server_referencedpaks;
extern int fs_server_referencedpaks[MAX_SEARCH_PATHS]; // checksums
extern char *fs_server_referenced_paknames[MAX_SEARCH_PATHS]; // pk3 names

extern struct cvar *fs_ignorelocalized;
extern struct cvar *fs_debug;
#define FS_DEBUG_PRINT(...) if (fs_debug->integer) { com_printf_dbginfo(__VA_ARGS__); }

extern struct cvar *fs_copyfiles;
extern struct cvar *fs_cdpath;
extern struct cvar *fs_basepath;
extern struct cvar *fs_basegame;
extern struct cvar *fs_homepath;
extern struct cvar *fs_gamedirvar;
extern struct cvar *fs_restrict;
extern struct cvar *fs_ignorelocalized;

//
//
// files/files.c
//
//
bool fs_pak_is_pure(struct pack *pack);
uint32_t fs_hash_filename(const char *fname, int hashsize);
filehandle fs_handle_for_file(void);
FILE *fs_file_for_handle(filehandle f);
int fs_file_length(filehandle f);
void fs_replace_separators(char *path);
bool fs_create_path(char *ospath);
bool fs_filename_compare(const char *s1, const char *s2);
char *fs_shifted_strstr(const char *string, const char *substring, int shift);
char *fs_shift_str(const char *string, int shift);
bool fs_search_localized(struct searchpath *search);
int fs_path_compare(const char *s1, const char *s2);
void fs_add_game_directory(const char *path, const char *dir);
void fs_clear_pak_references(int flags);

//
//
// files/files_cmds.c
//
//
void fs_path(bool ignorelocalized);

//
//
// files/io.c
//
//
struct pack *fs_load_zip_file(char *zipfile, const char *basename);
int fs_read_file(const char *qpath, void **buffer);
int fs_read(void *buffer, int len, filehandle f);
void fs_flush(filehandle f);

//
//
// files/io_sv.c
//
//
bool fs_sv_file_exists(const char *file);
filehandle fs_sv_fopen_file_write(const char *filename);
int fs_sv_fopen_file_read(const char *filename, filehandle *fp);
void fs_sv_rename(const char *from, const char *to);

#endif // __FILES_LOCAL_H__
