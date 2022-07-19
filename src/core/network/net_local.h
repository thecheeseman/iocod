/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

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
extern qbool winsock_init;

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
qbool net_string_to_sockaddr(const char *s, sa_family_t family,
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

void net_event(fd_set *fdr);

/**
 * @brief Get a packet.
 * @param[in] net_from address of the sender
 * @param[in] fdr      file descriptor set
 * @param[out] msg     message buffer
 * @return true if successful, false otherwise
*/
qbool net_get_packet(struct netadr *net_from, fd_set *fdr, struct netmsg *msg);

/* This is based on the Adaptive Huffman algorithm described in Sayood's Data
 * Compression book.  The ranks are not actually stored, but implicitly defined
 * by the location of a node within a doubly-linked list
 */

#define HMAX 256 /* Maximum symbol */
#define NYT HMAX					/* NYT = Not Yet Transmitted */
#define INTERNAL_NODE (HMAX+1)

typedef struct nodetype {
    /* tree structure */
    struct nodetype *left;
    struct nodetype *right;
    struct nodetype *parent;

    /* doubly-linked list */
    struct nodetype *next;
    struct nodetype *prev;

    /* highest ranked node in block */
    struct nodetype **head;
    int	weight;
    int	symbol;
} node_t;

typedef struct {
    int	blocNode;
    int	blocPtrs;

    node_t *tree;
    node_t *lhead;
    node_t *ltail;
    node_t *loc[HMAX + 1];
    node_t **freelist;

    node_t nodeList[768];
    node_t *nodePtrs[768];
} huff_t;

typedef struct {
    huff_t compressor;
    huff_t decompressor;
} huffman_t;

extern huffman_t msg_huffman;

void huff_compress(struct netmsg *buf, int offset);
void huff_decompress(struct netmsg *buf, int offset);
void huff_init(huffman_t *huff);
void huff_add_ref(huff_t *huff, byte ch);
int huff_receive(node_t *node, int *ch, byte *fin);
void huff_transmit(huff_t *huff, int ch, byte *fout, int maxoffset);
void huff_offset_receive(node_t *node, int *ch, byte *fin, int *offset, 
                         int maxoffset);
void huff_offset_transmit(huff_t *huff, int ch, byte *fout, int *offset, 
                          int maxoffset);
void huff_put_bit(int bit, byte *fout, int *offset);
int huff_get_bit(byte *fout, int *offset);
int	huff_get_bloc(void);
void huff_set_bloc(int _bloc);

void msg_init(struct netmsg *buf, byte *data, int length);
void msg_init_oob(struct netmsg *buf, byte *data, int length);
void msg_clear(struct netmsg *buf);

/** @} */

#endif /* NET_LOCAL_H */
