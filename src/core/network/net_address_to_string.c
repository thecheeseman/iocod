#include "net_local.h"

IC_PUBLIC
char *net_address_to_string(struct netadr a)
{
    static char s[MAX_ADDR_STRING];

    switch (a.type) {
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

            net_netadr_to_sockaddr(&a, (struct sockaddr *) &sadr);
            net_sockaddr_to_string(s, sizeof(s), (struct sockaddr *) &sadr);
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
char *net_address_to_string_port(struct netadr a)
{
    static char s[MAX_ADDR_STRING];

    switch (a.type) {
    case NA_IP:
        snprintf(s, sizeof(s), "%s:%hu", net_address_to_string(a), a.port);
        break;
    case NA_IP6:
        snprintf(s, sizeof(s), "[%s]:%hu", net_address_to_string(a), a.port);
        break;
    default:
        snprintf(s, sizeof(s), "%s", net_address_to_string(a));
        break;
    }

    return s;
}
