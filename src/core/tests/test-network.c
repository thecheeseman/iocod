#include "ic_test.h"

int TEST_MAIN()
{
    log_init();
    cv_init(); // needed in network
    net_init();

    net_shutdown();

    return 0;
}
