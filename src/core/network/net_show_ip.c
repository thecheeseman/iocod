#include "net_local.h"

IC_PUBLIC
void net_show_ip(void)
{
    for (int i = 0; i < numip; i++) {
        char s[MAX_ADDR_STRING];
        net_sockaddr_to_string(s, sizeof(s), 
                               (struct sockaddr *) &localip[i].addr);

        if (localip[i].type == NA_IP)
            log_print("IPv4: %s\n", s);
        else if (localip[i].type == NA_IP6)
            log_print("IPv6: %s\n", s);
    }
}
