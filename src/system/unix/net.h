#ifndef SYSTEM_UNIX_NET_H
#define SYSTEM_UNIX_NET_H

void net_init(void);
void net_shutdown(void);
void net_open_ip(void);
bool sys_string_to_sockaddr(const char *s, struct sockaddr *sadr);
void net_get_local_address(void);
int net_ip_socket(char *netif, int port);
void net_open_ip(void);

#endif /* SYSTEM_UNIX_NET_H */
