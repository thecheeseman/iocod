#include "iocod.h"
#include <stdlib.h>

IC_PUBLIC
IC_NO_RETURN
void sys_exit(int code)
{
    con_shutdown();
    net_shutdown();
    config_shutdown();

    exit(code);
}

IC_PUBLIC
void sys_quit(void)
{
    sys_exit(IC_OK);
}
