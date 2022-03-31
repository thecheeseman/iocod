#include <direct.h>

#include "shared.h"
#include "system/windows/local.h"

char *sys_cwd(void)
{
    static char cwd[MAX_OSPATH];

    (void) _getcwd(cwd, sizeof(cwd) - 1);
    cwd[MAX_OSPATH - 1] = '\0';

    return cwd;
}

char *sys_default_base_path(void)
{
    return sys_cwd();
}

char *sys_default_install_path(void)
{
    return sys_cwd();
}

char *sys_default_home_path(void)
{
    return NULL;
}

void sys_set_default_cd_path(const char *path)
{

}

char *sys_default_cd_path(void)
{
    return "";
}
