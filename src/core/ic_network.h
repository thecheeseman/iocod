#ifndef IC_NETWORK_H
#define IC_NETWORK_H

struct netmsg {
    bool allow_overflow;
    bool overflowed;
    bool oob;

    byte *data;

    int max_size;
    int cur_size;
    int uncomp_size;
    int read_count;
    int bit;
};

enum netadr_type {
    NA_BOT,
    NA_BAD,
    NA_LOOPBACK,
    NA_BROADCAST,
    NA_IP,
    NA_IP6
};

enum netsrc_type {
    NS_CLIENT,
    NS_SERVER
};

struct netadr {
    enum netadr_type type;

    byte ip[4];
    byte ip6[16];

    unsigned short port;
    unsigned long scope_id;
};

IC_PUBLIC
void net_init(void);

IC_PUBLIC
void net_shutdown(void);

IC_PUBLIC
void net_restart(void);

IC_PUBLIC
char *net_address_to_string(struct netadr a);

IC_PUBLIC
char *net_address_to_string_port(struct netadr a);

IC_PUBLIC
void net_show_ip(void);

IC_PUBLIC
bool net_is_lan_address(struct netadr adr);

#endif /* IC_NETWORK_H */
