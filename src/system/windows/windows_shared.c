//#include <lmerr.h>
//#include <lmcons.h>
//#include <lmwksta.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <stdio.h>
//#include <direct.h>
//#include <io.h>
//#include <conio.h>

#include <direct.h>

#include "shared.h"
#include "windows_local.h"

int sys_time_base;
int sys_milliseconds(void)
{
    int cur_time;
    static bool initialized = false;

    if (!initialized) {
        sys_time_base = timeGetTime();
        initialized = true;
    }

    cur_time = timeGetTime() - sys_time_base;
    return cur_time;
}

char *sys_get_current_user(void)
{
    static char username[1024];
    unsigned long size = sizeof(username);

    if (!GetUserName(username, &size))
        strcpy(username, "player");

    if (*username == '\0')
        strcpy(username, "player");

    return username;
}

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

void *sys_load_dll(const char *name, char *fqpath, 
                   intptr_t (**entrypoint)(intptr_t, ...),
                   intptr_t (*systemcalls)(intptr_t, ...))
{
    return NULL;
}

void sys_unload_dll(void *dllhandle)
{

}
