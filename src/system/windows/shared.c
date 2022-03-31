#include "system/windows/local.h"

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
