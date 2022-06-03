#include "iocod.h"

int main(void)
{
    log_init();
    net_init();

    net_shutdown();

    return 0;
}
