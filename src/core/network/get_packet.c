#include "net_local.h"

bool net_get_packet(struct netadr *net_from, struct netmsg *msg, fd_set *fdr)
{
    for (int protocol = 0; protocol < 2; protocol++) {
        socket_t sock;
        if (protocol == 0)
            sock = ip_socket;
        else if (protocol == 1)
            sock = ip6_socket;

        if (sock != INVALID_SOCKET && FD_ISSET(sock, fdr)) {
            struct sockaddr_storage from;
            socklen_t fromlen = sizeof(from);

            int ret = recvfrom(sock, (void *) msg->data, msg->max_size, 0,
                               (struct sockaddr *) &from, &fromlen);

            if (ret == SOCKET_ERROR) {
                int err = sockerror();
                if (err == EAGAIN || err != ECONNRESET)
                    log_print("%s", net_error_string());

                continue;
            }

            net_sockaddr_to_netadr((struct sockaddr *) &from, net_from);
            msg->read_count = 0;

            if (ret >= msg->max_size) {
                log_print(_("Oversize packet from '%s'"),
                          net_address_to_string(*net_from));
                return false;
            }

            msg->cur_size = ret;
            return true;
        }
    }

    return false;
}
