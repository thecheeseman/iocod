#include "system/shared.h"
#include "system/windows/console.h"
#include "system/windows/local.h"

void sys_quit(void)
{
    timeEndPeriod(1);
    in_shutdown();
    console_destroy();

    exit(0);
}
