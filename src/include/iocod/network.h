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

#ifndef IC_NETWORK_H
#define IC_NETWORK_H

/**
 * @defgroup network Network
 * @brief Public procedures and data structures for interfacing with the
 * network module.
 * 
 * @{
*/

struct netmsg {
    bool allow_overflow;
    bool overflowed;
    bool oob;

    byte *data;

    int max_size;
    int current_size;
    int uncompressed_size;
    int read_count;
    int bit;
};

/**
 * @brief Network address types.
*/
enum netadr_type {
    NA_BAD,         /**< bad address */
    NA_BOT,         /**< address is a (local) bot address */
    NA_LOOPBACK,    /**< address is a (local) loopback address */
    NA_BROADCAST,   /**< currently unused */
    NA_IP,          /**< IPv4 address */
    NA_IP6          /**< IPv6 address */
};

enum netsrc_type {
    NS_CLIENT,
    NS_SERVER
};

/**
 * @brief Network address structure used to hold IP/port information for the 
 * game system.
*/
struct netadr {
    enum netadr_type type;      /**< type of address */

    byte ip[4];                 /**< IPv4 address data */
    byte ip6[16];               /**< IPv6 address data */

    unsigned short port;        /**< port */
    unsigned long scope_id;     /**< IPv6 scope id, currently unused */
};

/**
 * @brief Initialize networking. 
 
 * @warning Network must be initialized before any subsequent calls to 
 * _any_ `net_` functions -- otherwise some functions will exhibit undefined
 * behavior.
 */
IC_PUBLIC
void net_init(void);

/**
 * @brief Shutdown networking.
 */
IC_PUBLIC
void net_shutdown(void);

/**
 * @brief Restart networking.
 */
IC_PUBLIC
void net_restart(void);

/**
 * @brief Convert @p addr to NULL-terminated string.
 * 
 * If the given net address is loopback, will return "loopback".
 * If the given net address is a bot, will return "bot".
 * Failures are indicated by a returned "BADADDR" string.
 * 
 * @param[in] addr address to convert
 * @return NULL-terminated string containing the address
*/
IC_PUBLIC
char *net_address_to_string(struct netadr addr);

/**
 * @brief Convert @p addr to NULL-terminated string with appended
 * port information.
 *
 * If the given net address is loopback, will return "loopback".
 * If the given net address is a bot, will return "bot".
 * Failures are indicated by a returned "BADADDR" string.
 *
 * @param[in] addr address to convert
 * @return NULL-terminated string containing the address and port
*/
IC_PUBLIC
char *net_address_to_string_port(struct netadr addr);

/**
 * @brief Show current, registered local IP addresses to the console.
 */
IC_PUBLIC
void net_show_ip(void);

/**
 * @brief Check if @p addr is a local address.
 * 
 * Local IPv4 addresses are in the ranges (RFC1918)
 * @code
 * 10.0.0.0/8       (10.0.0.0 - 10.255.255.255)
 * 127.0.0.0/8      (127.0.0.0 - 127.255.255.255)
 * 172.16.0.0/12    (172.16.0.0 - 172.31.255.255)
 * 192.168.0.0/16   (192.168.0.0 - 192.168.255.255)
 * @endcode
 * 
 * Local IPv6 addresse are in the ranges
 * @code
 * ::1/128
 * fe80::/10
 * @endcode
 * 
 * @param[in] addr `netadr` structure to check
 * @return true if address is a local address, false otherwise
*/
IC_PUBLIC
bool net_is_lan_address(struct netadr addr);

IC_PUBLIC
void net_sleep(int msec);

/**
 * @brief Send packet of length @p length, to address @p to.
 * @param[in] length length of packet to send
 * @param[in] data   pointer to packet data
 * @param[in] to     address to send packet to
 * @return true if packet was sent, false otherwise
*/
IC_PUBLIC
bool net_send_packet(int length, const void *data, struct netadr to);

/** @} */

#endif /* IC_NETWORK_H */
