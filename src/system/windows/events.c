#include "system/shared.h"
#include "system/windows/local.h"

void sys_events_init(void)
{

}

void sys_queue_event(int time, enum system_event_type type,
                     int value, int value2, int ptrlength, void *ptr)
{

}

struct system_event sys_get_event(void)
{
    struct system_event s;
    return s;
}
