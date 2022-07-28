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
#include <ctype.h>

u32 hash_filename(const char *filename, u32 hash_size)
{
    u32 hash = 0;
    u32 i = 0;
    while (filename[i] != '\0') {
        char c = tolower(filename[i]);

        if (c == '.')
            break;

        if (c == '\\' || c == '/')
            c = '/';

        hash += (long) (c) * (i + 119);
        i++;
    }

    hash = (hash ^ (hash >> 10) ^ (hash >> 20));
    hash &= (hash_size - 1);
    return hash;
}

IC_NON_NULL(1)
pack_t *fs_load_zip(_In_z_ const char *filename)
{
    mz_zip_archive *zip = ic_calloc(1, sizeof(*zip));
    if (zip == NULL) {
        ic_error("YIKES");
        return NULL;
    }

    mz_bool success = mz_zip_reader_init_file(zip, filename, 0);
    if (!success) {
        ic_error("unable to open file: %s\n", filename);
        ic_free(zip);
        return NULL;
    }

    u32 num_files = mz_zip_reader_get_num_files(zip);

    // count total filename lengths
    size_t len = 0;
    for (u32 i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat stat;

        if (!mz_zip_reader_file_stat(zip, i, &stat)) {
            ic_error("unable to stat file: %s\n", filename);
            mz_zip_reader_end(zip);
            ic_free(zip);
            return NULL;
        }

        len += strlen(stat.m_filename) + 1;
    }

    // allocate pack data + filenames in one large chunk
    packed_file_t *buf = ic_malloc((num_files * sizeof(packed_file_t)) + len);
    IC_ASSERT(buf != NULL);
    char *name_ptr = ((char *) buf) + num_files * sizeof(packed_file_t);

    // find next closest power of 2 size
    u32 hash_size = 1;
    for (; hash_size < MAX_PAK_HASH_SIZE; hash_size <<= 1) {
        if (hash_size > num_files)
            break;
    }

    pack_t *pak = 
        ic_malloc(sizeof(pack_t) + (hash_size * sizeof(packed_file_t)));
    IC_ASSERT(pak != NULL);

    // init hash table
    pak->hash_size = hash_size;
    pak->hash_table = (packed_file_t **) (((char *) pak) + sizeof(pack_t));

    for (u32 i = 0; i < hash_size; i++)
        pak->hash_table[i] = NULL;

    strncpyz(pak->filename, filename, sizeof(pak->filename));
    strncpyz(pak->basename, filename, sizeof(pak->basename));

    // strip extension
    size_t bnamelen = strlen(pak->basename);
    if (bnamelen > 4 && strcasecmp(pak->basename + (bnamelen - 4), ".pk3") == 0)
        pak->basename[bnamelen - 4] = '\0';

    pak->handle = zip;
    pak->num_files = num_files;

    // load file data into the structures
    for (u32 i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat stat;
        char fname[MAX_OSPATH] = { 0 };

        if (!mz_zip_reader_file_stat(zip, i, &stat)) {
            ic_error("unable to stat file: %s\n", filename);
            mz_zip_reader_end(zip);
            ic_free(zip);
            return NULL;
        }

        strncpyz(fname, stat.m_filename, sizeof(fname));
        strlwr(fname);

        u32 hash = hash_filename(fname, pak->hash_size);

        buf[i].name = name_ptr;
        strcpy(buf[i].name, fname);
        name_ptr += strlen(fname) + 1;

        buf[i].pos = stat.m_central_dir_ofs;
        buf[i].len = stat.m_comp_size;
        buf[i].next = pak->hash_table[hash];
        pak->hash_table[hash] = &buf[i];
    }

    pak->build_buffer = buf;
    return pak;
}
