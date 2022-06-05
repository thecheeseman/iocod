#include "net_local.h"

IC_PUBLIC
bool net_is_lan_address(struct netadr adr)
{
    if (adr.type == NA_LOOPBACK)
        return true;

    if (adr.type == NA_IP) {
        // RFC1918:
        // 10.0.0.0        -   10.255.255.255  (10/8 prefix)
        // 172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
        // 192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
        if (adr.ip[0] == 10)
            return true;
        else if (adr.ip[0] == 172 && (adr.ip[1] & 0xf0) == 16)
            return true;
        else if (adr.ip[0] == 192 && adr.ip[1] == 168)
            return true;
        else if (adr.ip[0] == 127)
            return true;
    } else if (adr.type == NA_IP6) {
        if (adr.ip6[0] == 0xfe && (adr.ip6[1] & 0xc0) == 0x80)
            return true;
        if ((adr.ip6[0] & 0xfe) == 0xfc)
            return true;
    }

    // compare against the networks this computer is member of
    for (int i = 0; i < numip; i++) {
        byte *compare_adr;
        byte *compare_mask;
        byte *compare_ip;
        int addrsize;

        if (localip[i].type == adr.type) {
            if (adr.type == NA_IP) {
                compare_ip = (byte *) 
                    &((struct sockaddr_in *) &localip[i].addr)->sin_addr.s_addr;
                compare_mask = (byte *) 
                    &((struct sockaddr_in *) &localip[i].netmask)->sin_addr.s_addr;
                compare_adr = adr.ip;

                addrsize = sizeof(adr.ip);
            } else if (adr.type == NA_IP6) {
                // TODO? should we check the scope_id here?

                compare_ip = (byte *) 
                    &((struct sockaddr_in6 *) &localip[i].addr)->sin6_addr;
                compare_mask = (byte *) 
                    &((struct sockaddr_in6 *) &localip[i].netmask)->sin6_addr;
                compare_adr = adr.ip6;

                addrsize = sizeof(adr.ip6);
            }

            bool differed = false;
            for (int run = 0; run < addrsize; run++) {
                if ((compare_ip[run] & compare_mask[run]) != 
                    (compare_adr[run] & compare_mask[run])) {
                    differed = true;
                    break;
                }
            }

            if (!differed)
                return true;
        }
    }

    return false;
}
