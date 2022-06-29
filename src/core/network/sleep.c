#include "net_local.h"

IC_PUBLIC
void net_sleep(int msec)
{
    if (msec < 0)
        msec = 0;

    fd_set fdr = {0};
    FD_ZERO(&fdr);

    socket_t highest = INVALID_SOCKET;
    if (ip_socket != INVALID_SOCKET) {
        FD_SET(ip_socket, &fdr);
        highest = ip_socket;
    } 
    
    if (ip6_socket != INVALID_SOCKET) {
        FD_SET(ip6_socket, &fdr);

        if (highest == INVALID_SOCKET || ip6_socket > highest)
            highest = ip6_socket;
    }

    #ifdef IC_PLATFORM_WINDOWS
    if (highest == INVALID_SOCKET) {
        SleepEx(msec, 0);
        return;
    }
    #endif

    struct timeval timeout = { 0 };
    timeout.tv_sec = msec / 1000;
    timeout.tv_usec = (msec % 1000) * 1000;

    int ret = select(highest + 1, &fdr, NULL, NULL, &timeout);

    if (ret == SOCKET_ERROR)
        log_warn("select() failed: %s\n", net_error_string());
    else
        net_event(&fdr);
}
