#include "net_local.h"

IC_PUBLIC
char *net_address_to_string(struct netadr a)
{
    static char s[64];
    char ip[64];

    switch (a.type) {
    case NA_LOOPBACK:
        snprintf(s, sizeof(s), "loopback");
        break;
    case NA_BOT:
        snprintf(s, sizeof(s), "bot");
        break;
    case NA_IPV4:
        snprintf(s, sizeof(s), "%i.%i.%i.%i:%hu",
                 a.ipv4[0], a.ipv4[1], a.ipv4[2], a.ipv4[3], a.port);
        break;
    case NA_IPV6:
        {
            char ipbuf[64];
            memset(&ipbuf, 0, sizeof(ipbuf));

            byte *p = &a.ipv6;
            for (int i = 0; i < 16; i += 2) {
                char tmp[6];

                if (p[i] == 0 && p[i + 1] == 0)
                    snprintf(tmp, sizeof(tmp), "0");
                else if (p[i] == 0)
                    snprintf(tmp, sizeof(tmp), "%x", p[i + 1]);
                else if (p[i] < 16)
                    snprintf(tmp, sizeof(tmp), "%x%2x", p[i], p[i + 1]);
                else
                    snprintf(tmp, sizeof(tmp), "%02x%02x", p[i], p[i + 1]);

                strcat(ipbuf, tmp);
                if (i < 14)
                    strcat(ipbuf, ":");
            }

            snprintf(s, sizeof(s), "[%s]:%hu", ipbuf, a.port);
        }
        break;
    case NA_BAD:
    default:
        snprintf(s, sizeof(s), "BADADDR");
        break;
    }

    return s;
}
