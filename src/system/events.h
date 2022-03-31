#ifndef SYSTEM_EVENTS_H
#define SYSTEM_EVENTS_H

#include "types/byte.h"

enum system_event_type {
    SE_NONE = 0,
    SE_KEY,
    SE_CHAR,
    SE_MOUSE,
    SE_JOYSTICK_AXIS,
    SE_CONSOLE,
    SE_PACKET,
    SE_BAD_EVENT
};

struct system_event {
    int time;
    enum system_event_type type;
    int	value;
    int value2;
    int	ptr_length; // bytes of data pointed to by ptr, for journaling
    void *ptr;		// this must be manually freed if not NULL
};

#define MAX_MSGLEN 16384

#define MAX_QUEUED_EVENTS 256
#define MASK_QUEUED_EVENTS (MAX_QUEUED_EVENTS - 1)

extern struct system_event event_queue[MAX_QUEUED_EVENTS];
extern int event_head;
extern int event_tail;
extern byte packet_received[MAX_MSGLEN];

// events
void events_init(void);
void queue_event(int time, enum system_event_type type,
                     int value, int value2, int ptrlength, void *ptr);
struct system_event get_event(void);

#endif /* SYSTEM_EVENTS_H */
