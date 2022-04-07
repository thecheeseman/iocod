#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> // bk001204
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <errno.h>

#include "shared.h"
#include "common.h"

#include "common/error.h"
#include "common/memory.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "strings/stringlib.h"
#include "system/shared.h"
#include "system/unix/net.h"
#include "types/byte.h"
#include "va.h"

#define PORT_SERVER 28960

#define MAX_IPS 16
static int numip;
static byte localip[MAX_IPS][4];

struct cvar *noudp;
int ip_socket;

void net_init(void)
{
    noudp = cvar_get("net_noudp", "0", 0);

    if (noudp->value == 0.0 && ip_socket == 0)
        net_open_ip();
}

void net_shutdown(void)
{
    if (ip_socket != 0) {
        close(ip_socket);
        ip_socket = 0;
    }
}

bool get_packet(struct netadr *from, struct msg *msg)
{
    return false;
}

bool sys_string_to_sockaddr(const char *s, struct sockaddr *sadr)
{
    struct hostent *h;

    com_memset(sadr, 0, sizeof(*sadr));
    ((struct sockaddr_in *) sadr)->sin_family = AF_INET;

    ((struct sockaddr_in *) sadr)->sin_port = 0;

    if (s[0] >= '0' && s[0] <= '9') {
        *(intptr_t *) &((struct sockaddr_in *) sadr)->sin_addr = inet_addr(s);
    } else {
        if ((h = gethostbyname(s)) == NULL)
            return false;

        *(intptr_t *) &((struct sockaddr_in *) sadr)->sin_addr =
            *(intptr_t *) h->h_addr_list[0];
    }

    return true;
}

void net_get_local_address(void)
{
    char hostname[256], *p;
    struct hostent *hostinfo;
    int ip, n;

    if (gethostname(hostname, 256) == -1)
        return;

    hostinfo = gethostbyname(hostname);
    if (hostinfo == NULL)
        return;

    com_printf("hostname: %s\n", hostinfo->h_name);
    n = 0;
    while ((p = hostinfo->h_aliases[n++]) != NULL)
        com_printf("alias: %s\n", p);

    if (hostinfo->h_addrtype != AF_INET)
        return;

    numip = 0;
    while ((p = hostinfo->h_addr_list[numip++]) != NULL && numip < MAX_IPS) {
        ip = ntohl(*(int *) p);

        localip[numip][0] = p[0];
        localip[numip][1] = p[1];
        localip[numip][2] = p[2];
        localip[numip][3] = p[3];

        com_printf("IP: %i.%i.%i.%i\n",
                   (ip >> 24) & 0xff,
                   (ip >> 16) & 0xff,
                   (ip >> 8) & 0xff,
                   ip & 0xff);
    }
}

#define PORT_ANY -1

int net_ip_socket(char *netif, int port)
{
    int newsock, i = 1;
    struct sockaddr_in address;
    int fionbioval = 1;

    if (netif != NULL)
        com_printf("Opening IP socket: %s:%i\n", netif, port);
    else
        com_printf("Opening IP socket: localhost:%i\n", netif, port);

    if ((newsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        com_printf("ERROR: udp_opensocket: socket: %s", net_error_string());
        return 0;
    }

    // open a non-blocking socket
    if (ioctl(newsock, FIONBIO, &fionbioval) == -1) {
        com_printf("ERROR: udp_opensocket: ioctl FIONBIO: %s\n",
                   net_error_string());
        return 0;
    }

    // broadcast capable
    if (setsockopt(newsock, SOL_SOCKET, SO_BROADCAST,
                   (char *) &i, sizeof(i)) == -1) {
        com_printf("ERROR: udp_opensocket: setsockopt SO_BROADCAST: %s\n",
                   net_error_string());
        return 0;
    }

    if (netif != NULL || *netif != '\0' || strcasecmp(netif, "localhost") == 0)
        address.sin_addr.s_addr = INADDR_ANY;
    else
        sys_string_to_sockaddr(netif, (struct sockaddr *) &address);

    if (port == PORT_ANY)
        address.sin_port = 0;
    else
        address.sin_port = htons((short) port);

    address.sin_family = AF_INET;

    if (bind(newsock, (void *) &address, sizeof(address)) == -1) {
        com_printf("ERROR: udp_opensocket: bind: %s\n", net_error_string());
        close(newsock);
        return 0;
    }

    return newsock;
}

void net_open_ip(void)
{
    struct cvar *ip;
    int port, i;

    ip = cvar_get("net_ip", "localhost", 0);
    port = cvar_get("net_port", va("%i", PORT_SERVER), 0)->value;

    for (i = 0; i < 10; i++) {
        ip_socket = net_ip_socket(ip->string, port + i);

        if (ip_socket != 0) {
            cvar_set_value("net_port", port + i);
            net_get_local_address();
            return;
        }
    }

    com_error(ERR_FATAL, "Couldn't allocate IP port");
}
