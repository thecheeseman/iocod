#include "iocod.h"

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

IC_PUBLIC
bool ic_append_extension(char *path, size_t size, const char *ext)
{
    if (path == NULL || size == 0 || ext == NULL)
        return false;

    const char *dot = strrchr(path, '.');
    if (dot != NULL && strcmp(dot, ext) == 0)
        return true; // already has the correct extension

    const char *slash = strrchr(path, '/');
    if (slash != NULL && path[strlen(path) - 1] == '/')
        return true; // path ends with a '/'

    // append correct extension
    char oldpath[PATH_MAX];
    strncpyz(oldpath, path, sizeof(oldpath));
    snprintf(path, size, "%s%s", oldpath, ext);

    return true;
}
