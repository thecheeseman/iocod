#include "ic_test.h"

int TEST_MAIN()
{
    ev_init();

    // insert all types of events
    ev_queue(0, SE_NONE, 0, 0, 0, NULL);
    ev_queue(0, SE_KEY, 1, 1, 0, NULL);
    ev_queue(0, SE_CHAR, 2, 2, 0, NULL);
    ev_queue(0, SE_MOUSE, 3, 3, 0, NULL);
    ev_queue(0, SE_JOYSTICK_AXIS, 4, 4, 0, NULL);
    ev_queue(0, SE_CONSOLE, 5, 5, 0, NULL);
    ev_queue(0, SE_PACKET, 6, 6, 0, NULL);
    ev_queue(0, SE_BAD_EVENT, 7, 7, 0, NULL);

    struct sys_event ev;

    #define EV_ASSERT(t) do { \
        ev = ev_get(); \
        IC_ASSERT(ev.type == t && ev.value == t && ev.value2 == t); \
    } while (0);

    EV_ASSERT(SE_NONE);
    EV_ASSERT(SE_KEY);
    EV_ASSERT(SE_CHAR);
    EV_ASSERT(SE_MOUSE);
    EV_ASSERT(SE_JOYSTICK_AXIS);
    EV_ASSERT(SE_CONSOLE);
    EV_ASSERT(SE_PACKET);
    EV_ASSERT(SE_BAD_EVENT);

    #undef EV_ASSERT

    // no more events, this should be blank
    ev = ev_get();
    IC_ASSERT(ev.type == SE_NONE);

    // push event test
    ev.type = SE_CHAR;
    ev.value = 'a';
    ev_push(&ev);

    struct sys_event ev2 = ev_get(); // should return pushed event
    IC_ASSERT(ev2.type == SE_CHAR && ev2.value == 'a');

    // insert bunch of events
    ev_queue(0, SE_KEY, 1, 1, 0, NULL);
    ev_queue(0, SE_CHAR, 2, 2, 0, NULL);
    ev_queue(0, SE_MOUSE, 3, 3, 0, NULL);
    ev_queue(0, SE_JOYSTICK_AXIS, 4, 4, 0, NULL);
    ev_queue(0, SE_CONSOLE, 5, 5, 0, NULL);
    ev_queue(0, SE_PACKET, 6, 6, 0, NULL);
    ev_queue(0, SE_BAD_EVENT, 7, 7, 0, NULL);

    // handle them all
    ev_loop();
}
