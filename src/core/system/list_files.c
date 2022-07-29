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

#include "iocod.h"

#define LIST_START_SIZE 128

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

struct file_list {
    const char *directory;
    const char *extension;
    const char *filter;
    qbool want_subs;
    
    char **list;
    u32 list_size;
    u32 num_files;

    #ifdef IC_PLATFORM_WINDOWS
    HANDLE find;
    WIN32_FIND_DATAW data;
    #else
    DIR *fdir;
    #endif
};

IC_NON_NULL(1)
static void list_add_file(_In_ struct file_list *list,
                          _In_opt_z_ const char *name)
{
    // grow the list if need be
    if (list->num_files + 1 >= list->list_size) {
        list->list_size *= 2;
        char **tmp = ic_realloc(list->list, list->list_size * sizeof(char *));
        if (tmp == NULL)
            ic_error("YIKES");

        list->list = tmp;
    }

    if (name != NULL)
        list->list[list->num_files] = copy_string(name);
    else
        list->list[list->num_files] = NULL;

    list->num_files++;
}

IC_NON_NULL(1)
static void list_shrink(_In_ struct file_list *list)
{
    char **tmp = ic_realloc(list->list, sizeof(char *) * list->num_files);
    if (tmp == NULL)
        ic_error("YIKES");

    list->list = tmp;
}

IC_NON_NULL(1)
static qbool list_open_folder(_In_ struct file_list *list)
{
    #ifdef IC_PLATFORM_WINDOWS
    wchar_t wdir[MAX_OSPATHW] = { 0 };
    wchar_t wext[MAX_OSPATHW] = { 0 };
    wchar_t wprepend[MAX_OSPATHW] = { 0 };

    utf8_widen(list->directory, wdir);
    utf8_widen(list->extension, wext);
    _snwprintf(wprepend, MAX_OSPATHW, L"\\\\?\\%s\\*%s", wdir, wext);

    list->find = FindFirstFileW(wprepend, &list->data);
    if (list->find == INVALID_HANDLE_VALUE) {
        ic_error("%s\n", sys_error_string());
        return false;
    }
    #else
    if ((list->fdir = opendir(list->directory)) == NULL) {
        ic_error("%s\n", sys_error_string());
        return false;
    }
    #endif

    return true;
}

IC_NON_NULL(1)
static void list_find_files(_In_ struct file_list *list)
{
    #ifdef IC_PLATFORM_WINDOWS
    char fname[MAX_OSPATH];
    do {
        if ((!list->want_subs && !(list->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) ||
            list->want_subs && (list->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (list->num_files == MAX_FOUND_FILES - 1)
                break;

            memset(fname, 0, sizeof(fname));
            utf16_shorten(list->data.cFileName, fname);
            list_add_file(list, fname);
        }
    } while (FindNextFileW(list->find, &list->data) != 0);

    FindClose(list->find);
    #else
    char search[MAX_OSPATH] = { 0 };
    struct dirent *d;
    struct stat st;
    while ((d = readdir(list->fdir)) != NULL) {
        snprintfz(search, sizeof(search), "%s/%s", list->directory, d->d_name);

        if (stat(search, &st) == -1)
            continue;

        if ((!list->want_subs && (st.st_mode & S_IFDIR)) ||
            (list->want_subs && !(st.st_mode & S_IFDIR))) {
            continue;
        }

        // wtf
        if (*(list->extension) != NULL &&
            ((strlen(d->d_name) < strlen(list->extension)) ||
             strcasecmp(d->d_name + strlen(d->d_name) -
                        strlen(list->extension), list->extension))) {
            continue;
        }

        if (list->num_files == MAX_FOUND_FILES - 1)
            break;

        list_add_file(list, d->d_name);
    }

    closedir(list->fdir);
    #endif
}

IC_NON_NULL(1)
static void list_filtered_files(_In_ struct file_list *list,
                                _In_z_ const char *basedir,
                                _In_z_ const char *subdir)
{
    UNUSED_PARAM(list);
    UNUSED_PARAM(basedir);
    UNUSED_PARAM(subdir);
}

IC_PUBLIC
IC_NON_NULL(1, 5)
char **sys_list_files(_In_z_ const char *directory,
                      _In_opt_z_ const char *extension,
                      _In_opt_z_ const char *filter,
                      qbool want_subs,
                      _Out_ u32 *total_files)
{
    *total_files = 0;

    if (extension == NULL)
        extension = "";
    
    struct file_list list = { 0 };
    list.directory = directory;
    list.extension = extension;
    list.filter = filter;
    list.want_subs = want_subs;
    list.list = NULL;
   
    // filter
    if (filter != NULL) {
        list_filtered_files(&list, list.directory, "");
        goto cleanup;
    }

    if (!list_open_folder(&list))
        return NULL;

    // allocate at least LIST_START_SIZE members
    // list_add_file will expand if necessary
    list.list = ic_malloc(sizeof(char *) * LIST_START_SIZE);
    list.list_size = LIST_START_SIZE;

    list_find_files(&list);

cleanup:
    // clean up
    if (list.num_files == 0) {
        ic_free(list.list);
        return NULL;
    }

    // insert blank at the end
    list_add_file(&list, NULL);
    
    // resize list so its size is num_files
    list_shrink(&list);

    *total_files = list.num_files - 1;
    return list.list;
}

IC_PUBLIC
IC_NON_NULL(1)
void sys_free_file_list(_In_ char **list)
{
    for (u32 i = 0; list[i] != NULL; i++)
        ic_free(list[i]);

    ic_free(list);
}
