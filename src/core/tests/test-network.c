#include "ic_test.h"

int TEST_MAIN()
{
    log_init();
    net_init();

    net_shutdown();

    return 0;
}
