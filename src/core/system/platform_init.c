#include "iocod.h"

static enum system_type systype = SYSTEM_UNKNOWN;

IC_PUBLIC
enum system_type sys_system_type(void)
{
    return systype;
}

IC_PUBLIC
bool sys_is_windows(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    return true;
    #else
    return false;
    #endif
}

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

IC_PUBLIC
void sys_platform_init(void)
{
    DWORD version = GetVersion();
    DWORD major = (DWORD) (LOBYTE(LOWORD(version)));
    DWORD minor = (DWORD) (HIBYTE(LOWORD(version)));

    // realistically we don't really care about the difference
    // between 2000, XP, server 2003, etc... but whatever
    if (major == 5) {
        if (minor == 0)
            systype = SYSTEM_WIN2000;
        else if (minor == 1)
            systype = SYSTEM_WINXP;
        else if (minor == 2)
            systype = SYSTEM_WIN2003;
    } else if (major == 6) {
        if (minor == 0)
            systype = SYSTEM_WINVISTA;
        else if (minor == 1)
            systype = SYSTEM_WIN7;
        else if (minor == 2)
            systype = SYSTEM_WIN8_OR_LATER;
    }
}
#else
IC_PUBLIC
void sys_platform_init(void)
{
    #ifdef IC_PLATFORM_MACOS
    systype = SYSTEM_MACOS;
    #else
    systype = SYSTEM_LINUX;
    #endif
}
#endif
