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

qbool pak_is_pure(pack_t *pak)
{
    return true;
}

IC_NON_NULL(1)
static qbool check_file_exists(_In_z_ const char *filename)
{
    UNUSED_PARAM(filename);
    
    for (searchpath_t *s = fs_searchpaths; s != NULL; s = s->next) {
        if (!use_localized_searchpath(s))
            continue;

        u32 hash = 0;
        if (s->pack != NULL)
            hash = hash_filename(filename, s->pack->hash_size);

        // if we have a pk3 file
        if (s->pack != NULL && s->pack->hash_table[hash] != NULL) {
            pack_t *pak = s->pack;
            packed_file_t *pakfile = pak->hash_table[hash];

            // search through this pk3's files
            do {
                if (filename_compare(pakfile->name, filename) == 0)
                    return true; // got it

                pakfile = pakfile->next;
            } while (pakfile != NULL);
        } else if (s->dir != NULL) {
            // we have a directory
            directory_t *dir = s->dir;

            char ospath[MAX_OSPATH] = { 0 };
            fs_build_ospath(dir->path, dir->game, filename, ospath, 0);

            FILE *tmp = fs_fopen(ospath, "rb");
            if (tmp == NULL)
                continue;

            fclose(tmp);
            return true;
        }
    }

    return false;
}

struct fread_data {
    const char *filename;
    filehandle_data_t *handle;
    qbool unique;

    searchpath_t *s;

    pack_t *pak;
    pack_t *unpure_pak;
    packed_file_t *pakfile;

    i64 search_pak_ret;
    i64 search_directory_ret;
};

static char *shifted_strstr(_In_z_ const char *string,
                            _In_z_ const char *substring,
                            i32 shift)
{
    char buf[MAX_STRING_TOKENS] = { 0 };
    i32 i = 0;

    for (; substring[i] != '\0'; i++)
        buf[i] = substring[i] + shift;

    buf[i] = '\0';
    return strstr(string, buf);
}

//
// search through this pk3 for the data->filename
//
qbool search_pak(struct fread_data *data)
{
    pack_t *pak = data->pak;
    packed_file_t *pakfile = data->pakfile;

    qbool found = false;
    do {
        // skip non matching file
        if (filename_compare(pakfile->name, data->filename) != 0) {
            pakfile = pakfile->next;
            continue;
        }

        // if (s->localized || pak_is_pure(data->pak))
        if (!data->s->localized && !pak_is_pure(data->pak)) {
            data->unpure_pak = data->pak;
            break;
        }

        // must be the file we want
        found = true;
        break;
    } while (pakfile != NULL);

    // didn't find it
    if (!found)
        return false;

    //
    //
    //
    size_t len = strlen(data->filename);

    // referenced stuff goes here
    // FROM IOQ3:
    // mark the pak as having been referenced and mark specifics on cgame and ui
    // shaders, txt, arena files  by themselves do not count as a reference as 
    // these are loaded from all pk3s 
    // from every pk3 file.. 
    if ((pak->referenced & FS_GENERAL_REF) == 0) {
        if (!extension_match(data->filename, ".shader", len) &&
            !extension_match(data->filename, ".txt", len) &&
            !extension_match(data->filename, ".cfg", len) &&
            !extension_match(data->filename, ".config", len) &&
            !extension_match(data->filename, ".arena", len) &&
            !extension_match(data->filename, ".menu", len) &&
            strstr(data->filename, "levelshots") == NULL) {
            pak->referenced |= FS_GENERAL_REF;
        }
    }

    // game_mp
    if ((pak->referenced & FS_QAGAME_REF) == 0 &&
        shifted_strstr(data->filename, SYS_DLLNAME_QAGAME, 
                       -SYS_DLLNAME_QAGAME_SHIFT)) {
        pak->referenced |= FS_QAGAME_REF;
    }

    // cgame_mp
    if ((pak->referenced & FS_CGAME_REF) == 0 &&
        shifted_strstr(data->filename, SYS_DLLNAME_CGAME,
                       -SYS_DLLNAME_CGAME_SHIFT)) {
        pak->referenced |= FS_CGAME_REF;
    }

    // ui_mp
    if ((pak->referenced & FS_UI_REF) == 0 &&
        shifted_strstr(data->filename, SYS_DLLNAME_UI,
                       -SYS_DLLNAME_UI_SHIFT)) {
        pak->referenced |= FS_UI_REF;
    }

    if (data->unique) {
        #if 0
        data->handle->handle.unique = true;

        // duplicate our archive handle
        mz_zip_archive *zip = ic_malloc(sizeof(*zip));
        mz_zip_zero_struct(zip);
        
        mz_bool success = mz_zip_reader_init_file_v2(zip, data->filename, 0,
                                                     pakfile->offset,
                                                     pak->handle->m_archive_size);
        if (!success) {
            ic_free(zip);
            ic_error("unable to open file %s: %s\n", data->filename,
                     mz_zip_get_error_string(zip->m_last_error));
            return false;
        }

        data->handle->handle.file.z = zip;
        #endif
    } else {
        data->handle->handle.file.z = pak->handle;
    }

    strncpyz(data->handle->name, data->filename, sizeof(data->handle->name));
    data->handle->zipfile = true;
    data->handle->zip_offset = pakfile->offset;
    data->search_pak_ret = pakfile->size_uncompressed;

    log_debug("%s (found in '%s')", data->filename, pak->basename);

    return true;
}

//
// check if the current data->filename exists at the directory location
//
static qbool search_directory(struct fread_data *data)
{
    directory_t *dir = data->s->dir;
    char ospath[MAX_OSPATH] = { 0 };
    fs_build_ospath(dir->path, dir->game, data->filename, ospath, 0);

    FILE *fp = fs_fopen(ospath, "rb");
    if (fp == NULL)
        return false;

    strncpyz(data->handle->name, data->filename, sizeof(data->handle->name));
    data->handle->zipfile = false;
    data->handle->handle.file.o = fp;
    data->search_directory_ret = fs_filep_length(fp);

    log_debug("%s (found in '%s%c%s')", data->filename,
              dir->path, PATH_SEP, dir->game);
    
    return true;
}

IC_PUBLIC
IC_NON_NULL(1)
i64 fs_fopen_file_read(_In_z_ const char *filename,
                       qbool unique,
                       _Out_opt_ filehandle *file)
{
    if (file == NULL)
        return check_file_exists(filename);

    // disallow leading slashes
    if (filename[0] == '/' || filename[0] == '\\')
        filename++;

    // make sure we cannot back up the path at all
    if (strstr(filename, "..") != NULL || strstr(filename, "::") != NULL) {
        *file = 0;
        return -1;
    }

    // codkey check
    // needed?

    *file = 0;
    filehandle handle = handle_for_file();
    if (handle == 0)
        return -1;

    struct fread_data data = { 0 };
    data.filename = filename;
    data.unique = unique;
    data.handle = &fs_handle[handle];

    // search for the file
    for (searchpath_t *s = fs_searchpaths; s != NULL; s = s->next) {
        // skip certain localized paks
        if (!use_localized_searchpath(s))
            continue;

        data.s = s;

        u32 hash = 0;
        if (s->pack != NULL)
            hash = hash_filename(filename, s->pack->hash_size);
        
        if (s->pack != NULL && s->pack->hash_table[hash] != NULL) {
            data.pak = s->pack;
            data.pakfile = s->pack->hash_table[hash];

            // try to find the file in this pk3
            if (search_pak(&data)) {
                // got it
                *file = handle;
                return data.search_pak_ret;
            }
        } else if (s->dir != NULL) {
            // try to find this in the directory
            if (search_directory(&data)) {
                // got it
                *file = handle;
                return data.search_directory_ret;
            }
        }

        // unpure check
        // probably don't need it until clientside but here it is
        if (data.unpure_pak != NULL) {
            ic_error(_("Unpure client detected. Invalid .pk3 files "
                       "referenced!\n %s"), data.unpure_pak->basename);
        }
    }

    // could not find the file
    return -1;
}
