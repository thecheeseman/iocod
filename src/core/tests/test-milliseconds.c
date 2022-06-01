#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

int main(void)
{
    IC_ASSERT(ic_milliseconds() == 0);

    #ifdef IC_PLATFORM_WINDOWS
    Sleep(1);
    #else
    usleep(1000);
    #endif

    IC_ASSERT(ic_milliseconds() > 0);

    return 0;
}
