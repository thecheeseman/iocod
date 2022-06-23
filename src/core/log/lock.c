#include "log_local.h"

void log_lock(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    EnterCriticalSection(&log_mutex);
    #else
    pthread_mutex_lock(&log_mutex);
    #endif
}

void log_unlock(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    LeaveCriticalSection(&log_mutex);
    #else
    pthread_mutex_unlock(&log_mutex);
    #endif
}
