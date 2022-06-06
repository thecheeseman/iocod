#ifndef NET_LOCAL_H
#define NET_LOCAL_H

#include "iocod.h"

/** @defgroup netlocal Network (local)
 * @brief Local (private) procedures and data structures for the
 * network module.
 *
 * @{
 */

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

/**
 * @def sockerror
 * @brief On Windows, this is equivalent to `WSAGetLastError()`, 
 * otherwise this is equivalent to `errno`.
 */

#ifdef IC_PLATFORM_WINDOWS
extern WSADATA winsockdata;
extern bool winsock_init;

typedef SOCKET socket_t;
typedef unsigned short sa_family_t;

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

/**
 * @brief Local address structure. From ioq3
 */
struct localaddr {
    char ifname[IF_NAMESIZE];           /**< interface name, if applicable */
    enum netadr_type type;              /**< address type */
    sa_family_t family;                 /**< address family 
                                             (AF_INET or AF_INET6) */
    struct sockaddr_storage addr;       /**< address */
    struct sockaddr_storage netmask;    /**< netmask */
};

/**
 * @def MAX_IPS
 * @brief Maximum allowed local ip addresses
 */
#define MAX_IPS 32

/**
 * @brief Local address structure array of @ref MAX_IPS IP addreses.
*/
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

/**
 * @brief Get a network error string.
 * @return null-terminated string containing the error message
*/
char *net_error_string(void);

/**
 * @brief Gets local addresses and populates @ref localip structure.
*/
void net_get_local_address(void);

/**
 * @brief Open IP sockets.
*/
void net_open_ip(void);

/**
 * @brief Convert @p s to a `sockaddr` structure.
 * 
 * @param[in]  s       string to convert 
 * @param[in]  family  `AF_INET` or `AF_INET6`
 * @param[in]  addrlen size of the sockaddr structure
 * @param[out] addr    converted `sockaddr` structure
 * @return true if successful, false otherwise
*/
bool net_string_to_sockaddr(const char *s, sa_family_t family,
                            socklen_t addrlen, struct sockaddr *addr);

/**
 * @brief Convert @p in to a NULL-termined string.
 * @param[in]  in      `sockaddr` structure
 * @param[in]  destsize size of destination string 
 * @param[out] dest     output string 
*/
void net_sockaddr_to_string(struct sockaddr *in, socklen_t destsize, 
                            char *dest);

/**
 * @brief Convert @p a to a sockaddr structure.
 * @param[in]  a `netadr` structure
 * @param[out] s `sockaddr` structure 
*/
void net_netadr_to_sockaddr(struct netadr *a, struct sockaddr *s);

/**
 * @brief Convert @p s to a @ref netadr structure.
 * @param[in]  s `sockadr` structure
 * @param[out] a `netaddr` structure
*/
void net_sockaddr_to_netadr(struct sockaddr *s, struct netadr *a);

/** @} */

#endif /* NET_LOCAL_H */
