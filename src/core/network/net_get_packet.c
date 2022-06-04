#include "net_local.h"

static void sockadr_to_netadr(struct sockaddr_in *s, struct netadr *a)
{
    *(int *) &a->ipv4 = *(int *) &s->sin_addr;
    a->port = s->sin_port;
    a->type = NA_IPV4;

    // TODO: ipv6
}

bool net_get_packet(struct netadr *net_from, struct netmsg *msg)
{
    int sock;

    for (int protocol = 0; protocol < 2; protocol++) {
        if (protocol == 0)
            sock = ipv4_sock;
        else
            sock = ipv6_sock;

        if (sock == 0)
            continue;

        struct sockaddr_in from;
        int fromlen = sizeof(from);
        int ret = recvfrom(sock, msg->data, msg->max_size, 0,
                           (struct sockaddr *) &from, &fromlen);

        if (ret == -1) {
            int err;
            
            #ifdef IC_PLATFORM_WINDOWS
            err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK || err == WSAECONNRESET)
                continue;
            #else
            err = errno;
            if (err == EWOULDBLOCK || ECONNREFUSED)
                continue;
            #endif

            log_debug("recvfrom: '%s' from '%s'\n", net_error_string(),
                      net_address_to_string(*net_from));
            continue;
        }

        // not sure if this is still required
        memset(((struct sockaddr_in *) &from)->sin_zero, 0, 8);

        sockadr_to_netadr(&from, net_from);
        msg->read_count = 0;

        if (ret == msg->max_size) {
            log_debug(_("Oversize packet from '%s'\n"),
                      net_address_to_string(*net_from));
            continue;
        }

        msg->cur_size = ret;
        return true;
    }

    return false;
}
