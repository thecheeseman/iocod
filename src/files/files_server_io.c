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
 * @file files_server_io.c
 * @date 2022-02-04
*/

#include "files_local.h"

/**
 * @brief
 * @param file
 * @return
*/
bool fs_sv_file_exists(const char *file)
{
	FILE *f;
	char testpath[MAX_OSPATH];

	fs_build_ospath(fs_homepath->string, file, "", testpath);

	f = fopen(testpath, "rb");
	if (f != NULL) {
		fclose(f);
		return true;
	}

	return false;
}

/**
 * @brief
 * @param filename
 * @return
*/
filehandle fs_sv_fopen_file_write(const char *filename)
{
	char ospath[MAX_OSPATH];
	filehandle f;

	if (fs_searchpaths == NULL)
		com_error(ERR_FATAL, "Filesystem call made without initialization");

	fs_build_ospath(fs_homepath->string, filename, "", ospath);
	FS_DEBUG_PRINT("%s\n", ospath);

	f = fs_handle_for_file();
	fsh[f].zipfile = false;

	if (fs_create_path(ospath))
		return 0;

	com_dprintf("writing to: %s\n", ospath);
	fsh[f].handlefiles.file.o = fopen(ospath, "wb");

	q_strncpyz(fsh[f].name, filename, sizeof(fsh[f].name));
	fsh[f].handlesync = false;

	if (fsh[f].handlefiles.file.o == NULL)
		f = 0;

	return f;
}

/**
 * @brief
 * @param filename
 * @param fp
 * @return
*/
int fs_sv_fopen_file_read(const char *filename, filehandle *fp)
{
	char ospath[MAX_OSPATH];
	filehandle f = 0;

	if (fs_searchpaths == NULL)
		com_error(ERR_FATAL, "Filesystem call made without initialization");

	f = fs_handle_for_file();
	fsh[f].zipfile = false;

	q_strncpyz(fsh[f].name, filename, sizeof(fsh[f].name));

	// don't let sound stutter -- useless on server?
	s_clear_sound_buffer();

	//ospath = fs_build_ospath(fs_homepath->string, filename, "");
	fs_build_ospath(fs_homepath->string, filename, "", ospath);
	FS_DEBUG_PRINT("%s\n", ospath);

	fsh[f].handlefiles.file.o = fopen(ospath, "rb");
	fsh[f].handlesync = false;

	if (fsh[f].handlefiles.file.o == NULL) {
		// search basepath
		if (q_stricmp(fs_homepath->string, fs_basepath->string) != 0) {
			fs_build_ospath(fs_basepath->string, filename, "", ospath);
			FS_DEBUG_PRINT("%s\n", ospath);

			fsh[f].handlefiles.file.o = fopen(ospath, "rb");
			fsh[f].handlesync = false;

			if (fsh[f].handlefiles.file.o == NULL)
				f = 0;
		}
	}

	if (fsh[f].handlefiles.file.o == NULL) {
		// search cdpath
		fs_build_ospath(fs_cdpath->string, filename, "", ospath);
		FS_DEBUG_PRINT("%s\n", ospath);

		fsh[f].handlefiles.file.o = fopen(ospath, "rb");
		fsh[f].handlesync = false;

		if (fsh[f].handlefiles.file.o == NULL)
			f = 0;
	}

	*fp = f;
	if (f != 0)
		return fs_file_length(f);

	return 0;
}

/**
 * @brief
 * @param from
 * @param to
*/
void fs_sv_rename(const char *from, const char *to)
{
	char from_ospath[MAX_OSPATH], to_ospath[MAX_OSPATH];

	if (fs_searchpaths == NULL)
		com_error(ERR_FATAL, "Filesystem call made without initialization");

	// don't let sound stutter -- useless on server?
	s_clear_sound_buffer();

	fs_build_ospath(fs_homepath->string, from, "", from_ospath);
	fs_build_ospath(fs_homepath->string, to, "", to_ospath);
	FS_DEBUG_PRINT("%s -> %s\n", from_ospath, to_ospath);

	if (rename(from_ospath, to_ospath)) {
		// failed to rename, try copying and deleting the original
		fs_copy_file(from_ospath, to_ospath);
		fs_remove(from_ospath);
	}
}
