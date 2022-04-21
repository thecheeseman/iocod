#include "fs_local.h"
#include "strings/stringlib.h"

#include <stdlib.h>

#ifdef PLATFORM_WINDOWS
#include <direct.h>
#include <io.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#define MAX_FOUND_FILES 65536

static void list_add_file(char ***list, size_t **nfiles, const char *name)
{
    char **list_copy = realloc(*list, ((**nfiles) + 1) * sizeof(char *));
    if (list_copy == NULL)
        com_error(ERR_FATAL, "error while reallocating memory");

    *list = list_copy;

    if (name != NULL)
        (*list)[**nfiles] = strdup(name);
    else
        (*list)[**nfiles] = NULL;

    (**nfiles)++;
}

static void list_filtered_files(const char *base_dir, const char *sub_dir,
                                char *filter, char ***list, size_t **num_files)
{
    char search[MAX_PATH];
    char new_sub_dir[MAX_PATH];

    #ifdef PLATFORM_WINDOWS
    int flag = 0;
    struct _finddata_t find_info;
    intptr_t find_handle;
    #else
    struct dirent *d;
    DIR *fdir;
    struct stat st;
    #endif

    if (**num_files >= MAX_FOUND_FILES - 1)
        return;

    if (strnlen(sub_dir, MAX_PATH) > 0) {
        #ifdef PLATFORM_WINDOWS
        snprintfz(search, sizeof(search), "%s\\%s\\*", base_dir, sub_dir);
        #else
        snprintfz(search, sizeof(search), "%s/%s", base_dir, sub_dir);
        #endif
    } else {
        #ifdef PLATFORM_WINDOWS
        snprintfz(search, sizeof(search), "%s\\*", base_dir);
        #else
        snprintfz(search, sizeof(search), "%s", base_dir);
        #endif
    }

    #ifdef PLATFORM_WINDOWS
    if ((find_handle = _findfirst(search, &find_info)) == -1)
        return NULL;

    do {
        if (!(find_info.attrib & _A_SUBDIR))
            continue;

        // recurse into directories
        if (strcasecmp(find_info.name, ".") != 0 &&
            strcasecmp(find_info.name, ".." != 0)) {
            if (strlen(sub_dir) > 0) {
                snprintfz(new_sub_dir, sizeof(new_sub_dir), "%s\\%s",
                            sub_dir, find_info.name);
            } else {
                snprintfz(new_sub_dir, sizeof(new_sub_dir), "%s",
                          find_info.name);
            }

            list_filtered_files(base_dir, new_sub_dir, filter, list,
                                num_files);
        }

        if (**num_files >= MAX_FOUND_FILES - 1)
            break;
    } while (_findnext(find_handle, &find_info) != -1);
    #else
        if ((fdir = opendir(base_dir)) == NULL)
        return NULL;
    #endif
}

char **fs_list_files(const char *directory, const char *extension,
                     char *filter, size_t *num_files, bool want_subs)
{
    char **list;
    char search[MAX_PATH];

    #ifdef PLATFORM_WINDOWS
    int flag = 0;
    struct _finddata_t find_info;
    intptr_t find_handle;
    #else
    struct dirent *d;
    DIR *fdir;
    struct stat st;
    bool dir_only = want_subs;
    #endif

    *num_files = 0;

    if (filter != NULL) {
        list = malloc(sizeof(*list));
        list_filtered_files(directory, "", filter, &list, &num_files);

        if (*num_files == 0) {
            free(list);
            return NULL;
        }

        // insert blank element at the end
        list_add_file(&list, &num_files, NULL);

        return list;
    }

    if (extension == NULL)
        extension = "";

    #ifdef PLATFORM_WINDOWS
    if (extension[0] == '/' && extension[1] == '\0') {
        extension = "";
        flag = 0;
    } else {
        flag = _A_SUBDIR;
    }

    snprintfz(search, sizeof(search), "%s\\*%s", directory, extension);

    if ((find_handle = _findfirst(search, &find_info)) == -1)
        return NULL;
    #else
    if (extension[0] == '/' && extension[1] == '\0') {
        extension = "";
        dir_only = true;
    }

    if ((fdir = opendir(directory)) == NULL)
        return NULL;
    #endif

    list = malloc(sizeof(*list));

    #ifdef PLATFORM_WINDOWS
    do {
        if ((!want_subs && (flag ^ (find_info.attrib & _A_SUBDIR))) ||
            (want_subs && (find_info.attrib & _A_SUBDIR))) {
            if (*num_files == MAX_FOUND_FILES - 1)
                break;

            list_add_file(&list, &num_files, find_info.name);
        }
    } while (_findnext(find_handle, &find_info) != -1);

    _findclose(find_handle);
    #else
    while ((d = readdir(fdir)) != NULL) {
        snprintfz(search, sizeof(search), "%s/%s", directory, d->d_name);
        if (stat(search, &st) == -1)
            continue;

        if ((dir_only && !(st.st_mode & S_IFDIR)) ||
            (!dir_only && (st.st_mode & S_IFDIR))) {
            continue;
        }

        if (*extension != NULL && 
            ((strlen(d->d_name) < strlen(extension)) ||
             strcasecmp(d->d_name + strlen(d->d_name) -
                        strlen(extension), extension))) {
            continue;
        }

        if (*num_files == MAX_FOUND_FILES - 1)
            break;

        list_add_file(&list, &num_files, d->d_name);
    }

    closedir(fdir);
    #endif

    if (*num_files == 0) {
        free(list);
        return NULL;
    }

    // insert blank element at the end
    list_add_file(&list, &num_files, NULL);

    return list;
}

void fs_free_file_list(char **list)
{
    if (list == NULL)
        return;

    for (size_t i = 0; list[i] != NULL; i++)
        free(list[i]);

    free(list);
}

void test_fs_file_list(void)
{
    size_t num_files;
    char **list = fs_list_files(".", "", NULL, &num_files, false);

    for (size_t i = 0; i < num_files; i++)
        printf("%d: %s\n", i, list[i]);

    fs_free_file_list(list);
}
