#include "net_local.h"

size_t num_ipv4 = 0;
byte local_ipv4[MAX_IPS][4];

size_t num_ipv6 = 0;
byte local_ipv6[MAX_IPS][16];

void net_get_local_address(void)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        log_warn("gethostname: %s\n", net_error_string());
        return;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    struct addrinfo *result = NULL;
    if (getaddrinfo(hostname, "0", &hints, &result) != 0) {
        log_warn("getaddrinfo: %s\n", net_error_string());
        freeaddrinfo(result);
        return;
    }

    struct addrinfo *ptr = NULL;
    int i = 0;
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
       
        switch (ptr->ai_family) {
        case AF_INET:
            {
                struct sockaddr_in *sockaddr_ipv4 =
                    (struct sockaddr_in *) ptr->ai_addr;
                unsigned long ip = ntohl(sockaddr_ipv4->sin_addr.s_addr);

                local_ipv4[num_ipv4][0] = (ip >> 24) & 0xff;
                local_ipv4[num_ipv4][1] = (ip >> 16) & 0xff;
                local_ipv4[num_ipv4][2] = (ip >> 8) & 0xff;
                local_ipv4[num_ipv4][3] = ip & 0xff;
                num_ipv4++;

                log_print("IPV4: %i.%i.%i.%i\n",
                          (ip >> 24) & 0xff,
                          (ip >> 16) & 0xff,
                          (ip >> 8) & 0xff,
                          ip & 0xff);
            }
            break;
        case AF_INET6:
            {
                struct sockaddr_in6 *sockaddr_ipv6 = 
                    (struct sockaddr_in6 *) ptr->ai_addr;

                char ipbuf[64];
                memset(&ipbuf, 0, sizeof(ipbuf));

                byte *p = sockaddr_ipv6->sin6_addr.s6_addr;
                for (int i = 0; i < 16; i += 2) {
                    char tmp[6];

                    if (p[i] == 0 && p[i + 1] == 0)
                        snprintf(tmp, sizeof(tmp), "0");
                    else if (p[i] == 0)
                        snprintf(tmp, sizeof(tmp), "%x", p[i + 1]);
                    else if (p[i] < 16)
                        snprintf(tmp, sizeof(tmp), "%x%2x", p[i], p[i + 1]);
                    else
                        snprintf(tmp, sizeof(tmp), "%02x%02x", p[i], p[i + 1]);

                    strcat(ipbuf, tmp);
                    if (i < 14)
                        strcat(ipbuf, ":");
                }

                log_print("IPV6: %s\n", ipbuf);
                
                memcpy(&local_ipv6[num_ipv6], p, 16);
                num_ipv6++;
            }
            break;
        default:
            break;
        }
    }

    freeaddrinfo(result);

    log_print(_("Total IPV4 addresses: %d\n"), num_ipv4);
    log_print(_("Total IPV6 addresses: %d\n"), num_ipv6);
}
