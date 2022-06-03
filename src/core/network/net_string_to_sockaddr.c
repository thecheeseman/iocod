#include "net_local.h"

bool net_string_to_sockaddr(const char *s, struct sockaddr *addr)
{
    memset(addr, 0, sizeof(*addr));

    ((struct sockaddr_in *) addr)->sin_family = AF_INET;
    ((struct sockaddr_in *) addr)->sin_port = 0;

    if (s[0] >= '0' && s[0] <= '9') {
        *(int *) &((struct sockaddr_in *) addr)->sin_addr = inet_addr(s);
        // hack
    } else {
        struct hostent *h;

        if ((h = gethostbyname(s)) == NULL)
            return false;

        *(int *) &((struct sockaddr_in *) addr)->sin_addr = 
            *(int *) h->h_addr_list[0];
        // hack
    }

    return true;
}
