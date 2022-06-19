#include "net_local.h"

IC_PUBLIC
char *net_address_to_string(struct netadr addr)
{
    static char s[MAX_ADDR_STRING];

    switch (addr.type) {
    case NA_LOOPBACK:
        snprintf(s, sizeof(s), "loopback");
        break;
    case NA_BOT:
        snprintf(s, sizeof(s), "bot");
        break;
    case NA_IP:
    case NA_IP6:
        {
            struct sockaddr_storage sadr;

            memset(&sadr, 0, sizeof(sadr));

            net_netadr_to_sockaddr(&addr, (struct sockaddr *) &sadr);
            net_sockaddr_to_string((struct sockaddr *) &sadr, sizeof(s), s);
        }
        break;
    case NA_BAD:
    default:
        snprintf(s, sizeof(s), "BADADDR");
        break;
    }

    return s;
}

IC_PUBLIC
char *net_address_to_string_port(struct netadr addr)
{
    static char s[MAX_ADDR_STRING];

    switch (addr.type) {
    case NA_IP:
        snprintf(s, sizeof(s), "%s:%hu", net_address_to_string(addr), 
                 addr.port);
        break;
    case NA_IP6:
        snprintf(s, sizeof(s), "[%s]:%hu", net_address_to_string(addr), 
                 addr.port);
        break;
    default:
        snprintf(s, sizeof(s), "%s", net_address_to_string(addr));
        break;
    }

    return s;
}
