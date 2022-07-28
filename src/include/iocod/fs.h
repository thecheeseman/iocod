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

#ifndef IOCOD_FS_H
#define IOCOD_FS_H

#include "iocod.h"

/**
 * @defgroup filesystem Filesystem
 * @brief File system module.
 * 
 * @{
 */

extern cvar_t *cl_language;

#define BASEGAME    "main"
#define DEFAULTCFG  "default_mp.cfg"

/**
 * @def MAX_OSPATH
 * @brief Maximum length of an OS path string.
 * 
 * @note This is lower than both Windows/Linux's MAX_PATH/PATH_MAX, but 
 * realistically will be sufficient unless we're running in a ton of 
 * subdirectories.
 */
#define MAX_OSPATH          256

/**
 * @def MAX_OSPATHW
 * @brief Maximum length of an OS path wide string (Windows only).
 */
#define MAX_OSPATHW         512

/**
 * @def MAX_PAK_FILES
 * @brief Maximum number of pk3 files.
 */
#define MAX_PAK_FILES       1024

/**
 * @def MAX_PAK_HASH_SIZE
 * @brief Maximum number of files in any given pk3 file.
 */
#define MAX_PAK_HASH_SIZE   4096

/**
 * @def MAX_FOUND_FILES
 * @brief Maximum number of files that can be found in a directory via
 * @ref sys_list_files().
 */
#define MAX_FOUND_FILES     4096

IC_PUBLIC
void fs_init(void);

IC_PUBLIC
void fs_shutdown(qbool a);

/** @} */

#endif /* IOCOD_FS_H */
