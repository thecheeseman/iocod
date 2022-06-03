#include "net_local.h"

#ifdef IC_PLATFORM_WINDOWS
static char *get_wsa_error_string(int code)
{
    static char msg[4096];

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                  msg, sizeof(msg), NULL);

    if (*msg == '\0')
        snprintf(msg, sizeof(msg), "WSAGetLastError: %d", code);

    return msg;
}
#endif

char *net_error_string(void)
{
    int code;

    #ifdef IC_PLATFORM_WINDOWS
    code = WSAGetLastError();
    return get_wsa_error_string(code);
    #else
    code = errno;
    return strerror(code);
    #endif
}
