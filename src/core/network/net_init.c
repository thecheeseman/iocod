#include "net_local.h"

#ifdef IC_PLATFORM_WINDOWS
WSADATA winsockdata;
bool winsock_init = false;
#endif

IC_PUBLIC
void net_init(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    int r = WSAStartup(MAKEWORD(1, 1), &winsockdata);
    if (r != 0) {
        log_error("WSAStartup: %s\n", net_error_string());
        return;
    }

    winsock_init = true;
    log_print("Winsock initialized\n");
    #endif

    net_open_ip();
}

IC_PUBLIC
void net_shutdown(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    if (!winsock_init)
        return;

    WSACleanup();
    winsock_init = false;
    #endif

    if (ipv4_sock != 0) {
        closesocket(ipv4_sock);
        ipv4_sock = 0;
    }
}
