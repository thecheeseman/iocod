#ifndef NET_LOCAL_H
#define NET_LOCAL_H

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#define PORT_ANY (-1)
#define PORT_SERVER (28960)

#define MAX_PORT_TRIES (32) /**< how many (PORT_SERVER + i) combinations 
                                 to try before giving up*/

#define MAX_IPS 32

#ifndef IC_PLATFORM_WINDOWS
#define ioctlsocket ioctl
#define closesocket close
#endif

#ifdef IC_PLATFORM_WINDOWS
extern WSADATA winsockdata;
extern bool winsock_init;
#endif

extern int ipv4_sock;
extern int ipv6_sock;

char *net_error_string(void);

void net_get_local_address(void);

bool net_open_ip(void);

bool net_string_to_sockaddr(const char *s, struct sockaddr *addr);

#endif /* NET_LOCAL_H */
