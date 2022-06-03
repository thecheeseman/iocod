#include "net_local.h"

int num_ipv4;
byte local_ipv4[MAX_IPS][4];

void net_get_local_address(void)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        log_warn("gethostname: %s\n", net_error_string());
        return;
    }

    struct hostent *hostinfo = gethostbyname(hostname);
    if (hostinfo == NULL) {
        log_warn("hostinfo: %s\n", net_error_string());
        return;
    }

    log_print("Hostname: %s\n", hostinfo->h_name);
    int n = 0;
    char *p;
    while ((p = hostinfo->h_aliases[n++]) != NULL)
        log_print("Alias: %s\n", p);

    if (hostinfo->h_addrtype != AF_INET)
        return;

    int num_ipv4 = 0;
    while ((p = hostinfo->h_addr_list[num_ipv4++]) != NULL && 
           num_ipv4 < MAX_IPS) {
        int ip = ntohl(*(int *) p);

        local_ipv4[num_ipv4][0] = p[0];
        local_ipv4[num_ipv4][1] = p[1];
        local_ipv4[num_ipv4][2] = p[2];
        local_ipv4[num_ipv4][3] = p[3];

        log_print("IP: %i.%i.%i.%i\n", 
                  (ip >> 24) & 0xff, 
                  (ip >> 16) & 0xff, 
                  (ip >> 8) & 0xff, 
                  ip & 0xff);
    }
}
