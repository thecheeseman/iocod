#include "shared.h"
#include "common.h"
#include "common/memory.h"
#include "strings/stringlib.h"
#include "system/events.h"
#include "system/shared.h"
#include "system/windows/console.h"
#include "system/windows/local.h"

extern void com_quit_f(void);

struct system_event get_event(void)
{
    MSG msg;
    struct system_event ev;
    char *s, *b;
    int len;
    struct msg netmsg;
    struct netadr adr, *buf;

    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(event_tail - 1) & MASK_QUEUED_EVENTS];
    }

    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
        if (!GetMessage(&msg, NULL, 0, 0))
            com_quit_f();

        window_vars.msg_time = msg.time;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    s = console_input();
    if (s != NULL) {
        len = strlen(s) + 1;
        b = z_malloc(len);
        strncpyz(b, s, len - 1);
        queue_event(0, SE_CONSOLE, 0, 0, len, b);
    }

    msg_init(&netmsg, packet_received, sizeof(packet_received));
    if (get_packet(&adr, &netmsg)) {
        len = sizeof(struct netadr) + netmsg.cursize - netmsg.readcount;
        buf = z_malloc(len);
        *buf = adr;

        memcpy(buf + 1, &netmsg.data[netmsg.readcount],
               netmsg.cursize - netmsg.readcount);
        queue_event(0, SE_PACKET, 0, 0, len, buf);
    }

    if (event_head > event_tail) {
        event_tail++;
        return event_queue[(event_tail - 1) & MASK_QUEUED_EVENTS];
    }

    memset(&ev, 0, sizeof(ev));
    ev.time = timeGetTime();

    return ev;
}
