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

static int path_compare(const char *s1, const char *s2)
{
    int c1 = 0;
    int c2 = 0;

    do {
        c1 = *s1++;
        c2 = *s2++;

        if (c1 >= 'a' && c1 <= 'z')
            c1 -= ('a' - 'A');
        if (c2 >= 'a' && c2 <= 'z')
            c2 -= ('a' - 'A');

        if (c1 == '\\' || c1 == ':')
            c1 = '/';
        if (c2 == '\\' || c2 == ':')
            c2 = '/';

        if (c1 < c2)
            return -1;      // strings not equal

        if (c1 > c2)
            return 1;

    } while (c1);

    return 0;       // strings are equal
}

static int pak_sort(const void *a, const void *b)
{
    char *aa = *(char **) a;
    char *bb = *(char **) b;

    return path_compare(aa, bb);
}

static qbool valid_localized_pak(_In_z_ const char *path,
                                 _In_z_ const char *pak,
                                 _Out_ u32 *localized_id)
{
    char *language = lz_get_pak_language(pak);
    if (*language == '\0') {
        ic_warning(_("Localized assets pak file %s/%s has invalid name "
                        "(no language specified)"), path, pak);
        return false;
    }
    
    u32 id;
    if (!lz_get_language_index(language, &id)) {
        ic_warning(_("Localized assets pak file %s/%s has invalid name "
                        "(bad language name specified)"), path, pak);
        static qbool warned = false;

        if (!warned) {
            lz_print_available_languages();
            warned = true;
        }

        return false;
    }
    *localized_id = id;

    return true;
}

IC_NON_NULL(1, 2)
void find_pack_files(_In_z_ const char *path,
                     _In_z_ const char *newdir)
{
    char ospath[MAX_OSPATH] = { 0 };

    fs_build_ospath(path, newdir, "", ospath, 0);
    ospath[strlen(ospath) - 1] = '\0'; // strip trailing slash

    if (!sys_directory_has_contents(ospath))
        return;

    u32 num_files;
    char **pak_files = sys_list_files(ospath, ".pk3", NULL, false, &num_files);

    if (num_files > MAX_PAK_FILES) {
        ic_warning("Exceeded max number of pk3 files in %s/%s (%d/%d)", 
                   path, newdir, num_files, MAX_PAK_FILES);
        num_files = MAX_PAK_FILES; // clamp to max
    }

    // sort pk3s
    char *sorted[MAX_PAK_FILES] = { 0 };
    for (u32 i = 0; i < num_files; i++) {
        sorted[i] = pak_files[i];

        // hack to sort localized paks at the end
        if (strncmp(sorted[i], "localized_", 10) == 0)
            memcpy(sorted[i], "          ", 10);
    }

    qsort(sorted, num_files, sizeof(char *), pak_sort);

    //
    // load pk3s
    //
    for (u32 i = 0; i < num_files; i++) {
        qbool localized = false;
        u32 localized_id = 0;

        // localized paks
        if (strncmp(sorted[i], "          ", 10) == 0) {
            memcpy(sorted[i], "localized_", 10);

            if (!valid_localized_pak(newdir, sorted[i], &localized_id))
                continue;

            localized = true;
        }

        char pakfile[MAX_OSPATH] = { 0 };
        fs_build_ospath(path, newdir, sorted[i], pakfile, 0);

        pack_t *pak = fs_load_zip(pakfile);
        if (pak != NULL) {
            strncpyz(pak->path, ospath, sizeof(pak->path));
            strncpyz(pak->gamename, newdir, sizeof(pak->gamename));

            fs_packed_files += pak->num_files;

            searchpath_t *sp = ic_malloc(sizeof(searchpath_t));
            sp->pack = pak;
            sp->dir = NULL;
            sp->localized = localized;
            sp->language = localized_id;
            add_search_path(sp);
        }
    }

    sys_free_file_list(pak_files);
}
