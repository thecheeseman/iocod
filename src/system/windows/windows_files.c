#include "types/filehandle.h"
#include "windows_local.h"

void sys_mkdir(const char *path)
{

}

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

}

char **sys_list_files(const char *directory, const char *extension,
                      char *filter, int *numfiles, bool wantsubs)
{
    *numfiles = 0;
    return NULL;
}

void sys_free_file_list(char **list)
{

}
