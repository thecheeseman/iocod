#include "common/memory.h"
#include "files/file_list.h"
#include "files/files_local.h"
#include "strings/stringlib.h"
#include "system/shared.h"

int get_mod_list(char *list, int size)
{
    return 0;
}

static int return_path(const char *zname, char *zpath, int *depth)
{
    int new_depth = 0;
    int len = 0;
    int i;

    *zpath = '\0';

    for (i = 0; zname[i] != '\0'; i++) {
        if (zname[i] == '/' || zname[i] == '\\') {
            len = i;
            new_depth++;
        }
    }

    strcpy(zpath, zname);
    zpath[len] = '\0';

    if (len + 1 == i)
        new_depth--;

    *depth = new_depth;
    return len;
}

static int add_file_to_list(char *name, char *list[MAX_FOUND_FILES],
                            int num_files)
{
    int i;

    if (num_files == MAX_FOUND_FILES - 1)
        return num_files;

    for (i = 0; i < num_files; i++) {
        if (strcasecmp(name, list[i]) == 0)
            return num_files; // already in list
    }

    list[num_files] = strdup(name);
    num_files++;

    return num_files;
}

char **list_filtered_files(const char *path, const char *extension,
                              char *filter, int *total)
{
    int num_files, num_sysfiles;
    char **listcpy, **sysfiles;
    char *list[MAX_FOUND_FILES];
    char *name;
    struct searchpath *search;
    int i, pathlen, extlen, len, depth, temp, zzz;
    struct pack *pak;
    struct fileinpack *build;
    char zpath[MAX_ZPATH], netpath[MAX_ZPATH];
    int zpathlen, zdepth;
    bool trailing_slash = false;

    if (path == NULL) {
        *total = 0;
        return NULL;
    }

    if (extension == NULL)
        extension = "";

    // skip leading slashes
    if (*path == '/' || *path == '\\')
        path++;

    trailing_slash = (strcasecmp(extension, "/") == 0);

    pathlen = strlen(path); 
    if (pathlen != 0 && (path[pathlen - 1] == '\\' || path[pathlen - 1] == '/'))
        pathlen--;

    extlen = strlen(extension);
    num_files = 0;
    return_path(path, zpath, &depth);

    if (*path != '\0')
        depth++;

    for (search = fs_searchpaths; search != NULL; search = search->next) {
        if (!fs_search_localized(search))
            continue;

        if (search->pack != NULL) {
            if (search->localized == 0 && !fs_pak_is_pure(search->pack))
                continue;

            pak = search->pack;
            build = pak->buildbuffer;
            for (i = 0; i < pak->numfiles; i++) {
                name = build[i].name;

                if (name == NULL)
                    continue;

                if (filter != NULL) {
                    if (!com_filter_path(filter, name, false))
                        continue;

                    num_files = add_file_to_list(name, list, num_files);
                    continue;
                }

                zpathlen = return_path(name, zpath, &zdepth);

                if (zdepth != depth || pathlen > zpathlen ||
                    name[pathlen] != '/' ||
                    strncasecmp(name, path, pathlen) != 0) {
                    continue;
                }

                len = strlen(name);
                if (len < extlen)
                    continue;

                if (strcasecmp(name + len - extlen, extension) != 0)
                    continue;

                temp = pathlen;
                if (pathlen != 0)
                    temp++; // include the '/'

                num_files = add_file_to_list(name + temp, list, num_files);
            }
        } else if (search->dir != NULL) {
            if (fs_restrict->integer > 0 || fs_num_serverpaks > 0)
                continue;

            if (strcasecmp(extension, "svg") == 0) { // not sure what this is
                fs_build_ospath(search->dir->path, search->dir->gamedir, path, 
                                netpath);
                sysfiles = sys_list_files(netpath, extension, filter,
                                          &num_sysfiles, false);
                for (i = 0; i < num_sysfiles; i++) {
                    name = sysfiles[i];
                    num_files = add_file_to_list(name, list, num_files);
                }

                sys_free_file_list(sysfiles);
            }
        }
    }

    *total = num_files;

    if (!num_files)
        return NULL;

    listcpy = z_malloc((num_files + 1) * sizeof(*listcpy));
    for (i = 0; i < num_files; i++)
        listcpy[i] = list[i];
    listcpy[i] = NULL;

    return listcpy;
}

char **list_files(const char *path, const char *extension, int *num_files)
{
    return list_filtered_files(path, extension, NULL, num_files);
}

void free_file_list(char **list)
{
    int i;

    if (list == NULL)
        return;

    for (i = 0; list[i]; i++)
        z_free(list[i]);

    z_free(list);
}

int get_file_list(const char *path, const char *extension, char *list, 
                     int size)
{
    int num_files, i, total, len;
    char **files = NULL;

    *list = '\0';
    num_files = 0;
    total = 0;

    if (strcasecmp(path, "$modlist") == 0)
        return get_mod_list(list, size);

    files = list_files(path, extension, &num_files);

    for (i = 0; i < num_files; i++) {
        len = strlen(files[i]) + 1;
        if (total + len + 1 < size) {
            strcpy(list, files[i]);
            list += len;
            total += len;
        } else {
            num_files = i;
            break;
        }
    }

    free_file_list(files);
    return num_files;
}
