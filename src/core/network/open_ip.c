#include "net_local.h"

static socket_t open_socket(int family, char *netif, int port, int *err)
{
    if (family == AF_INET) {
        log_print(_("Opening IPv4 socket: %s:%i"),
                  netif != NULL ? netif : "0.0.0.0",
                  port);
    } else if (family == AF_INET6) {
        log_print(_("Opening IPv6 socket: [%s]:%i"),
                  netif != NULL ? netif : "::", 
                  port);
    } 

    // create the socket
    socket_t newsock;
    if ((newsock = socket(family, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        log_warn("%s", net_error_string());

        *err = sockerror();
        return INVALID_SOCKET;
    }

    // make it non-blocking
    unsigned long _true = true;
    if (ioctlsocket(newsock, FIONBIO, &_true) == SOCKET_ERROR) {
        log_warn("ioctlsocket: %s", net_error_string());

        *err = sockerror();
        return INVALID_SOCKET;
    }

    // ipv4/ipv6 options
    int i = 1;
    if (family == AF_INET) {
        if (setsockopt(newsock, SOL_SOCKET, SO_BROADCAST,
                       (char *) &i, sizeof(i)) == SOCKET_ERROR) {
            log_warn("setsockopt: %s", net_error_string());
            return INVALID_SOCKET;
        }
    } else if (family == AF_INET6) {
        #ifdef IPV6_V6ONLY 
        if (setsockopt(newsock, IPPROTO_IPV6, IPV6_V6ONLY,
                       (char *) &i, sizeof(i)) == SOCKET_ERROR) {
            log_warn("setsockopt: %s", net_error_string());
            // non-fatal
        }
        #endif
    }

    struct sockaddr_in addr4 = {0};
    struct sockaddr_in6 addr6 = {0};
    void *addr = NULL;
    int addrsize = 0;
    
    if (family == AF_INET) {
        addr4.sin_family = AF_INET;

        if (netif == NULL || *netif == '\0') {
            addr4.sin_addr.s_addr = INADDR_ANY;
        } else {
            if (!net_string_to_sockaddr(netif, AF_INET, sizeof(addr4), 
                                        (struct sockaddr *) &addr4)) {
                closesocket(newsock);
                return INVALID_SOCKET;
            }
        }

        if (port == PORT_ANY)
            addr4.sin_port = 0;
        else
            addr4.sin_port = htons((short) port);

        addr = &addr4;
        addrsize = sizeof(addr4);
    } else if (family == AF_INET6) {
        addr6.sin6_family = AF_INET6;

        if (netif == NULL || *netif == '\0') {
            addr6.sin6_addr = in6addr_any;
        } else {
            if (!net_string_to_sockaddr(netif, AF_INET6, sizeof(addr6), 
                                        (struct sockaddr *) &addr6)) {
                closesocket(newsock);
                return INVALID_SOCKET;
            }
        }

        if (port == PORT_ANY)
            addr6.sin6_port = 0;
        else
            addr6.sin6_port = htons((short) port);

        addr = &addr6;
        addrsize = sizeof(addr6);
    }

    if (bind(newsock, addr, addrsize) == SOCKET_ERROR) {
        log_warn("bind: %s", net_error_string());

        *err = sockerror();
        closesocket(newsock);
        return INVALID_SOCKET;
    }

    return newsock;
}

void net_open_ip(void)
{
    int err;

    net_get_local_address();

    // IPV6
    if (net_enabled->integer & NET_ENABLE_IPV6) {
        int port6 = net_port6->integer;

        for (int i = 0; i < MAX_PORT_TRIES; i++) {
            ip6_socket = open_socket(AF_INET6, net_ip6->string, port6 + i, &err);

            if (ip6_socket != INVALID_SOCKET) {
                cv_set_integer("net_port6", port6 + i);
                break;
            } else {
                // no ipv6 support, just cancel
                if (err == EAFNOSUPPORT)
                    break;
            }
        }

        if (ip6_socket == INVALID_SOCKET)
            log_warn(_("Could not bind to an IPv6 address"));
    }

    // IPV4
    if (net_enabled->integer & NET_ENABLE_IPV4) {
        int port4 = net_port->integer;

        for (int i = 0; i < MAX_PORT_TRIES; i++) {
            ip_socket = open_socket(AF_INET, net_ip->string, port4 + i, &err);

            if (ip_socket != INVALID_SOCKET) {
                cv_set_integer("net_port", port4 + i);
                break;
            } else {
                // no ipv4 support? wat
                if (err == EAFNOSUPPORT)
                    break;
            }
        }

        if (ip_socket == INVALID_SOCKET)
            log_warn(_("Could not bind to an IPv4 address"));
    }
}
