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
    NA_IPV4,
    NA_IPV6
};

enum netsrc_type {
    NS_CLIENT,
    NS_SERVER
};

struct netadr {
    enum netadr_type type;

    byte ipv4[4];
    byte ipv6[16];

    unsigned short port;
};

IC_PUBLIC
void net_init(void);

IC_PUBLIC
void net_shutdown(void);

IC_PUBLIC
char *net_address_to_string(struct netadr a);

#endif /* IC_NETWORK_H */
