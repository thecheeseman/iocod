#include "fs_local.h"

#ifdef PLATFORM_WINDOWS
#include <direct.h>
#else
#include <unistd.h>
#endif

char *fs_getcwd(void)
{
    static char cwd[MAX_PATH];

    (void) getcwd(cwd, sizeof(cwd) - 1);
    cwd[MAX_PATH - 1] = '\0';

    return cwd;
}
