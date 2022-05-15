#include "iocod.h"

#define MAX_PATH 256

IC_PUBLIC
void default_extension(char *path, size_t size, const char *ext)
{
    char oldpath[MAX_PATH];
    
    /* if path doesn't have an extension, append */
    char *src = path + strlen(path) - 1;

    /* walk backwards through the string */
    while (*src != '/' && src != path) {
        if (*src == '.')
            return; /* already has an extension */

        src--;
    }

    strncpyz(oldpath, path, sizeof(oldpath));
    snprintf(path, size, "%s%s", oldpath, ext);
}
