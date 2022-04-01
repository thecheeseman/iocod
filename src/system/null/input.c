#include "shared.h"
#include "common.h"

void in_init(void)
{

}

void in_frame(void)
{

}

void in_shutdown(void)
{

}

void send_key_events(void)
{

}

void in_mouse_event(int state)
{

}

void in_activate(bool active)
{

}

void in_restart_f(void)
{
    in_shutdown();
    in_init();
}
