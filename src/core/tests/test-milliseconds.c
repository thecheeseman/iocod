#include "ic_test.h"

int TEST_MAIN()
{
    IC_ASSERT(sys_milliseconds() == 0);

    #ifdef IC_PLATFORM_WINDOWS
    Sleep(1);
    #else
    usleep(1000);
    #endif

    IC_ASSERT(sys_milliseconds() > 0);

    return 0;
}
