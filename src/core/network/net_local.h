#ifndef NET_LOCAL_H
#define NET_LOCAL_H

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <wspiapi.h>
#include <ws2spi.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#define NET_ENABLE_IPV4 0x01
#define NET_ENABLE_IPV6 0x02
#define NET_ENABLE_PRIOV6 0x04

#define PORT_ANY (-1)
#define PORT_SERVER (28960)

#define MAX_PORT_TRIES 16 /**< how many (PORT_SERVER + i) combinations 
                               to try before giving up*/

#define MAX_ADDR_STRING 128

#ifdef IC_PLATFORM_WINDOWS
extern WSADATA winsockdata;
extern bool winsock_init;

typedef SOCKET socket_t;
typedef unsigned short sa_family_t;
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT WSAEAFNOSUPPORT
#endif

#define sockerror() WSAGetLastError()
#else
typedef int socket_t;

#define ioctlsocket ioctl
#define closesocket close

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define sockerror() errno
#endif

#ifndef IF_NAMESIZE
#define IF_NAMESIZE 16
#endif

// 
struct localaddr {
    char ifname[IF_NAMESIZE];
    enum netadr_type type;
    sa_family_t family;
    struct sockaddr_storage addr;
    struct sockaddr_storage netmask;
};
#define MAX_IPS 32

extern struct localaddr localip[MAX_IPS];
extern int numip;

// sockets
extern socket_t ip_socket;
extern socket_t ip6_socket;

// cvars
extern struct cvar *net_enabled;
extern struct cvar *net_ip;
extern struct cvar *net_ip6;
extern struct cvar *net_port;
extern struct cvar *net_port6;

char *net_error_string(void);

void net_get_local_address(void);

void net_open_ip(void);

bool net_string_to_sockaddr(const char *s, struct sockaddr *addr,
                            socklen_t addrlen, sa_family_t family);
void net_sockaddr_to_string(char *dest, int size, struct sockaddr *in);
void net_netadr_to_sockaddr(struct netadr *a, struct sockaddr *s);
void net_sockaddr_to_netadr(struct sockaddr *s, struct netadr *a);

#endif /* NET_LOCAL_H */
