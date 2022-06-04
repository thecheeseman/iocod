#include "net_local.h"

int ipv4_sock = 0;
int ipv6_sock = 0;

static int open_socket(char *netif, int port, int family)
{
    log_print(_("Opening IP socket: %s:%i\n"), 
              netif != NULL ? netif : "localhost", 
              port);

    int newsock;
    if ((newsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        log_warn("%s\n", net_error_string());
        return 0;
    }

    bool _true = true;
    if (ioctlsocket(newsock, FIONBIO, &_true) == -1) {
        log_warn("ioctlsocket: %s\n", net_error_string());
        return 0;
    }

    int i = 1;
    if (setsockopt(newsock, SOL_SOCKET, SO_BROADCAST,
                   (char *) &i, sizeof(i)) == -1) {
        log_warn("setsockopt: %s\n", net_error_string());
        return 0;
    }

    struct sockaddr_in addr;
    if (netif == NULL || *netif == '\0' || strcasecmp(netif, "localhost") == 0)
        addr.sin_addr.s_addr = INADDR_ANY;
    else
        net_string_to_sockaddr(netif, (struct sockaddr *) &addr);

    if (port == PORT_ANY)
        addr.sin_port = 0;
    else
        addr.sin_port = htons((short) port);

    addr.sin_family = family;

    if (bind(newsock, (void *) &addr, sizeof(addr)) == -1) {
        log_warn("bind: %s\n", net_error_string());
        closesocket(newsock);
        return 0;
    }

    return newsock;
}

bool net_open_ip(void)
{
    struct cvar *net_ip = cv_get("net_ip", "localhost", CV_LATCH);
    struct cvar *net_port = cv_get("net_port", va("%i", PORT_SERVER), CV_LATCH);

    // automatically scan for the next available port
    int port = net_port->integer;
    for (int i = 0; i < MAX_PORT_TRIES; i++) {
        ipv4_sock = open_socket(net_ip->string, port + i, AF_INET);

        if (ipv4_sock != 0) {
            cv_set_value("net_port", port + i);

            net_get_local_address();
            return true;
        }
    }

    log_error(_("Couldn't allocate IP port\n"));
    return false;
}
