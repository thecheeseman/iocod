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
 * @brief Maximum number of hash table entries per pk3 file.
 * 
 * For the 9 stock 1.1 pk3s (localized_english0-1 + pak0-6)
 * 29625 total files = average of 3292 files per pk3
 * 
 * Size     memory usage
 * 4096     27334608 bytes
 * 2048     17831888 bytes
 * 1024      9517008 bytes
 * 
 * With a random assortment of pk3s from my mods folder (48 pk3s)
 * 43852 total files = average of 914 files per pk3
 * 
 * Size     memory usage
 * 4096     50792176 bytes
 * 2048     38913776 bytes
 * 1024     24659696 bytes
 * 
 * Realistically on newer system RAM usage isn't really a problem, so 
 * maybe upping this value can increase lookup performance.
 */
#define MAX_PAK_HASH_SIZE   1024

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

IC_PUBLIC
void fs_fclose_file(filehandle handle);

/**
 * @brief Load file at @p path, reading contents into @p buffer. If checking
 * for file existence, set @p buffer to NULL. 
 * 
 * @param[in] path    path to file
 * @param[out] buffer buffer to load file into, or NULL to get the size
 * @return size of file, or -1 on error
*/
IC_PUBLIC
IC_NON_NULL(1)
i64 fs_read_file(_In_z_ const char *path,
                 _Out_opt_ void **buffer);

/** @} */

#endif /* IOCOD_FS_H */
