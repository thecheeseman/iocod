#include "windows_local.h"

void sys_print(const char *msg)
{
    console_append(msg);
}
