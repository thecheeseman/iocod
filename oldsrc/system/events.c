#include "common/memory.h"
#include "common/print.h"
#include "system/events.h"
#include "system/shared.h"

struct system_event event_queue[MAX_QUEUED_EVENTS];
int event_head = 0;
int event_tail = 0;
byte packet_received[MAX_MSGLEN];

void events_init(void)
{
    com_memset(&event_queue[0], 0, MAX_QUEUED_EVENTS *
               sizeof(struct system_event));
    com_memset(&packet_received[0], 0, MAX_MSGLEN * sizeof(byte));
}

void queue_event(int time, enum system_event_type type,
                     int value, int value2, int ptr_length, void *ptr)
{
    struct system_event *ev;

    ev = &event_queue[event_head & MASK_QUEUED_EVENTS];

    if (event_head - event_tail >= MASK_QUEUED_EVENTS) {
        com_printf("sys_queue_event: overflow\n");

        if (ev->ptr)
            z_free(ev->ptr);

        event_tail++;
    }

    event_head++;

    if (time == 0)
        time = sys_milliseconds();

    ev->time = time;
    ev->type = type;
    ev->value = value;
    ev->value2 = value2;
    ev->ptr_length = ptr_length;
    ev->ptr = ptr;
}
