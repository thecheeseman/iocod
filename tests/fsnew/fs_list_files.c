#include "fs.h"

#ifdef IC_PLATFORM_WINDOWS
#include "dirent.h"
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

#include <errno.h>

#define MAX_FOUND_FILES 65536

IC_PUBLIC
void fs_list_filtered_files(const char *dir, char *subdirs, char *filter,
                            char **list, size_t *num_files, 
                            size_t *realloc_size)
{
    
}

IC_PUBLIC
char **fs_list_files(const char *dir, const char *ext, char *filter,
                     bool want_subdirs, size_t *num_files)
{
    *num_files = 0;
    size_t nfiles = 0;
    char **list;
    size_t realloc_size = 32;

    /* hook for fs_list_filtered_files */
    if (filter != NULL) {
        list = ic_malloc(sizeof(*list) + realloc_size * sizeof(char *));
        fs_list_filtered_files(dir, "", filter, list, &nfiles, &realloc_size);

        goto cleanup;
    }

    if (ext == NULL)
        ext == "";

    bool dir_only = want_subdirs;
    if (ext[0] == '/' && ext[1] == '\0') {
        ext = "";
        dir_only = true;
    }
    size_t extlen = strlen(ext);

    DIR *fdir;
    if ((fdir = opendir(dir)) == NULL) {
        ic_warning("opendir: %s\n", strerror(errno));
        return NULL;
    }

    list = ic_malloc(sizeof(*list) + realloc_size * sizeof(char *));
    struct dirent *d;

    while ((d = readdir(fdir)) != NULL) {
        char search[MAX_PATH_LEN];
        struct stat st;

        /* reached our limit */
        if (nfiles == MAX_FOUND_FILES - 1)
            break;

        /* check if this file is accessible or not */
        snprintfz(search, sizeof(search), "%s/%s", dir, d->d_name);
        if (stat(search, &st) == -1) {
            ic_warning("stat: %s\n", strerror(errno));
            continue;
        }

        /* 
         * skip if:
         * a) this is a directory and we don't want directories
         * b) this isn't a directory and we want directories
         */
        if ((dir_only && !(st.st_mode & S_IFDIR)) ||
            (!dir_only && (st.st_mode & S_IFDIR))) {
            continue;
        }

        /* check if the files only differ by extension */
        size_t dlen = strlen(d->d_name);
        if (*ext != NULL &&
            ((dlen < extlen) ||
             strcasecmp(d->d_name + dlen - extlen, ext) != 0)) {
            continue;
        }

        /* if needed, realloc array to hold enough elements */
        if (nfiles == realloc_size - 1) {
            realloc_size <<= 1;
            if (realloc_size > MAX_FOUND_FILES - 1)
                realloc_size = MAX_FOUND_FILES;

            ic_printf("realloc to %d elements\n", realloc_size);

            char **list_copy = ic_realloc(list, realloc_size * sizeof(char *));
            if (list_copy == NULL) {
                ic_error("couldn't reallocate list of size %lu\n", 
                         realloc_size * sizeof(char *));
                break;
            }

            list = list_copy;
        }

        /* add to list */
        list[nfiles] = strdup(d->d_name);
        nfiles++;
    }

    closedir(fdir);

cleanup:
    /* nothing found */
    if (nfiles == 0) {
        ic_free(list);
        return NULL;
    }

    /* shrink the array down to the correct size */
    char **list_copy = ic_realloc(list, (nfiles + 1) * sizeof(char *));
    if (list_copy == NULL) {
        ic_error("couldn't reallocate list of size %lu\n",
                 (nfiles + 1) * sizeof(char *));

        ic_free(list);
        return NULL;
    }

    list = list_copy;
    list[nfiles] = NULL;
    *num_files = nfiles;

    return list;
}

IC_PUBLIC
void fs_free_file_list(char **list)
{
    if (list == NULL)
        return;

    for (size_t i = 0; list[i] != NULL; i++)
        ic_free(list[i]);

    ic_free(list);
}
