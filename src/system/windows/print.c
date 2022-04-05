#include "system/windows/console.h"
#include "system/windows/local.h"

void sys_print(const char *msg)
{
    console_append(msg);
}
