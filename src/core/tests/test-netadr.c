#include "ic_test.h"

int TEST_MAIN()
{
    struct netadr a;
    memset(&a, 0, sizeof(a));

    a.type = NA_BAD;
    IC_ASSERT(strcmp(net_address_to_string(a), "BADADDR") == 0);

    a.type = NA_BOT;
    IC_ASSERT(strcmp(net_address_to_string(a), "bot") == 0);
    
    a.type = NA_LOOPBACK;
    IC_ASSERT(strcmp(net_address_to_string(a), "loopback") == 0);

    a.type = NA_IPV4;
    memcpy(&a.ipv4, (byte[4]) { 0xc0, 0xa8, 0x0, 0x2a }, 4);
    a.port = 28960;
    IC_ASSERT(strcmp(net_address_to_string(a), "192.168.0.42:28960") == 0);

    memset(&a, 0, sizeof(a));
    a.type = NA_IPV6;
    memcpy(&a.ipv6, (byte[16]) { 0x20, 0x01, 0x00, 0x00, 0x32, 0x38, 0xdf, 0xe1, 
           0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfb }, 16);
    a.port = 20510;
    /*
    IC_ASSERT(strcmp(net_address_to_string(a), 
                     "[2001:0:3238:dfe1:63::fefb]:20510") == 0);
    */

    return 0;
}
