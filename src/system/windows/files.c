#include <stddef.h>
#include <io.h>
#include "shared.h"
#include "common.h"
#include "common/memory.h"
#include "strings/stringlib.h"
#include "types/bool.h"
#include "types/filehandle.h"

void sys_end_streamed_file(filehandle f)
{

}

bool sys_directory_has_files(const char *ospath)
{
    return false;
}

void sys_list_filtered_files(const char *basedir, char *subdirs, char *filter,
                             char **list, int *numfiles)
{
    char search[MAX_OSPATH], newsubdirs[MAX_OSPATH];
    char filename[MAX_OSPATH];
    int handle;
    struct _finddata_t findinfo;

    if (*numfiles >= MAX_FOUND_FILES - 1)
        return;

    if (strlen(subdirs) > 0)
        snprintfz(search, sizeof(search), "%s\\%s\\*", basedir, subdirs);
    else
        snprintfz(search, sizeof(search), "%s\\*", basedir);

    handle = _findfirst(search, &findinfo);
    if (handle == -1)
        return;

    do {
        if (findinfo.attrib & _A_SUBDIR) {
            if (strcasecmp(findinfo.name, ".") != 0 &&
                strcasecmp(findinfo.name, "..") != 0) {
                if (strlen(subdirs) > 0) {
                    snprintfz(newsubdirs, sizeof(newsubdirs), "%s\\%s",
                                subdirs, findinfo.name);
                } else {
                    snprintfz(newsubdirs, sizeof(newsubdirs), "%s",
                                findinfo.name);
                }

                sys_list_filtered_files(basedir, newsubdirs, filter, list, 
                                        numfiles);
            }

            if (*numfiles >= MAX_FOUND_FILES - 1)
                break;

            snprintfz(filename, sizeof(filename), "%s\\%s", subdirs,
                        findinfo.name);
            if (!com_filter_path(filter, filename, false))
                continue;

            list[*numfiles] = strdup(filename);
            (*numfiles)++;
        }
    } while (_findnext(handle, &findinfo) != -1);

    _findclose(handle);
}

static bool strgtr(const char *s0, const char *s1)
{
    int l0, l1, i;

    l0 = strlen(s0);
    l1 = strlen(s1);

    if (l1 < l0)
        l0 = l1;

    for (i = 0; i < l0; i++) {
        if (s1[i] > s0[i])
            return true;

        if (s1[i] < s0[i])
            return false;
    }

    return false;
}

char **sys_list_files(const char *directory, const char *extension,
                      char *filter, int *numfiles, bool wantsubs)
{
    char search[MAX_OSPATH];
    int nfiles;
    char **listcpy;
    char *list[MAX_FOUND_FILES];
    struct _finddata_t findinfo;
    intptr_t findhandle;
    int flag;
    int i;

    if (filter != NULL) {
        nfiles = 0;
        sys_list_filtered_files(directory, "", filter, list, &nfiles);

        list[nfiles] = 0;
        *numfiles = nfiles;

        if (!nfiles) {
            return NULL;
        }

        listcpy = z_malloc((nfiles + 1) * sizeof(*listcpy));
        for (i = 0; i < nfiles; i++)
            listcpy[i] = list[i];
        listcpy[i] = NULL;

        return listcpy;
    }

    if (extension == NULL)
        extension = "";

    // passing a slash as extension will find directories
    if (extension[0] == '/' && extension[1] == 0) {
        extension = "";
        flag = 0;
    } else {
        flag = _A_SUBDIR;
    }

    snprintfz(search, sizeof(search), "%s\\*%s", directory, extension);

    // search
    nfiles = 0;

    findhandle = _findfirst(search, &findinfo);
    if (findhandle == -1) {
        *numfiles = 0;
        return NULL;
    }

    do {
        if ((!wantsubs && flag ^ (findinfo.attrib & _A_SUBDIR)) || 
            (wantsubs && findinfo.attrib & _A_SUBDIR)) {
            if (nfiles == MAX_FOUND_FILES - 1)
                break;

            list[nfiles] = strdup(findinfo.name);
            nfiles++;
        }
    } while (_findnext(findhandle, &findinfo) != -1);

    list[nfiles] = 0;

    _findclose(findhandle);

    // return a copy of the list
    *numfiles = nfiles;

    if (!nfiles)
        return NULL;

    listcpy = z_malloc((nfiles + 1) * sizeof(*listcpy));
    for (i = 0; i < nfiles; i++)
        listcpy[i] = list[i];
    listcpy[i] = NULL;

    do {
        flag = 0;
        for (i = 1; i < nfiles; i++) {
            if (strgtr(listcpy[i - 1], listcpy[i])) {
                char *temp = listcpy[i];
                listcpy[i] = listcpy[i - 1];
                listcpy[i - 1] = temp;
                flag = 1;
            }
        }
    } while (flag);

    return listcpy;
}

void sys_free_file_list(char **list)
{
    int i;

    if (list == NULL)
        return;

    for (i = 0; list[i]; i++)
        z_free(list[i]);

    z_free(list);
}
