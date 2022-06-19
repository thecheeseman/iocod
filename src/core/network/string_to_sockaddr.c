#include "net_local.h"

static struct addrinfo *search_addrinfo(struct addrinfo *hints,
                                        sa_family_t family)
{
    while (hints != NULL) {
        if (hints->ai_family == family)
            return hints;

        hints = hints->ai_next;
    }

    return NULL;
}

bool net_string_to_sockaddr(const char *s, sa_family_t family, 
                            socklen_t addrlen, struct sockaddr *addr)
{
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    memset(addr, 0, sizeof(*addr));

    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM;
    bool ret = false;

    struct addrinfo *result = NULL;
    if (getaddrinfo(s, NULL, &hints, &result) != 0) {
        log_warn(_("Error resolving '%s': %s"), s, net_error_string());
        goto end;
    }

    struct addrinfo *search = NULL;
    if (family == AF_UNSPEC) {
        // didn't specify which family to use, so let's decide
        if (net_enabled->integer & NET_ENABLE_PRIOV6) {
            // ipv6 priority
            if (net_enabled->integer & NET_ENABLE_IPV6)
                search = search_addrinfo(result, AF_INET6);

            if (search == NULL && (net_enabled->integer & NET_ENABLE_IPV4))
                search = search_addrinfo(result, AF_INET);
        } else {
            // ipv4 priority
            if (net_enabled->integer & NET_ENABLE_IPV4)
                search = search_addrinfo(result, AF_INET);

            if (search == NULL && (net_enabled->integer & NET_ENABLE_IPV6))
                search = search_addrinfo(result, AF_INET6);
        }
    } else {
        // do what is specified
        search = search_addrinfo(result, family);
    }

    if (search == NULL) {
        log_warn(_("Error resolving '%s': No address of required type found"), 
                 s);
        goto end;
    }

    if (search->ai_addrlen > addrlen)
        search->ai_addrlen = addrlen;

    memcpy(addr, search->ai_addr, search->ai_addrlen);
    ret = true;

end:
    freeaddrinfo(result);
    return ret;
}

void net_sockaddr_to_string(struct sockaddr *in, socklen_t destsize, 
                            char *dest)
{
    socklen_t addrlen;

    if (in->sa_family == AF_INET)
        addrlen = sizeof(struct sockaddr_in);
    else if (in->sa_family == AF_INET6)
        addrlen = sizeof(struct sockaddr_in6);

    if (getnameinfo(in, addrlen, dest, destsize, NULL, 0, NI_NUMERICHOST) != 0 
        && addrlen > 0) {
        *dest = '\0';
    }
}
