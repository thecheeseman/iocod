#include "net_local.h"

void net_netadr_to_sockaddr(struct netadr *a, struct sockaddr *s)
{
    if (a->type == NA_BROADCAST) {
        ((struct sockaddr_in *) s)->sin_family = AF_INET;
        ((struct sockaddr_in *) s)->sin_port = a->port;
        ((struct sockaddr_in *) s)->sin_addr.s_addr = INADDR_BROADCAST;
    } else if (a->type == NA_IP) {
        ((struct sockaddr_in *) s)->sin_family = AF_INET;
        ((struct sockaddr_in *) s)->sin_addr.s_addr = *(int *) &a->ip;
        ((struct sockaddr_in *) s)->sin_port = a->port;
    } else if (a->type == NA_IP6) {
        ((struct sockaddr_in6 *) s)->sin6_family = AF_INET6;
        ((struct sockaddr_in6 *) s)->sin6_addr = *((struct in6_addr *) &a->ip6);
        ((struct sockaddr_in6 *) s)->sin6_port = a->port;
        ((struct sockaddr_in6 *) s)->sin6_scope_id = a->scope_id;
    }
}

void net_sockaddr_to_netadr(struct sockaddr *s, struct netadr *a)
{
    if (s->sa_family == AF_INET) {
        a->type = NA_IP;

        *(int *) &a->ip = ((struct sockaddr_in *) s)->sin_addr.s_addr;

        a->port = ((struct sockaddr_in *) s)->sin_port;
    } else if (s->sa_family == AF_INET6) {
        a->type = NA_IP6;

        memcpy(a->ip6, &((struct sockaddr_in6 *) s)->sin6_addr, sizeof(a->ip6));

        a->port = ((struct sockaddr_in6 *) s)->sin6_port;
        a->scope_id = ((struct sockaddr_in6 *) s)->sin6_scope_id;
    }
}
