#include "log_local.h"

intptr_t log_current_thread_id(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    return (intptr_t) GetCurrentThreadId();
    #else
    return (intptr_t) pthread_self();
    #endif
}
