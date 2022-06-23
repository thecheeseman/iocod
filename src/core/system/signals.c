#include "iocod.h"
#include <signal.h>

static void sys_signal_handler(int signal)
{
    static bool caught = false;

    if (caught) {
        fprintf(stderr, "double signal fault: received %d, exiting...", signal);
    } else {
        caught = true;
        
        // cl shutdown
        // sv shutdown
    }

    if (signal == SIGTERM || signal == SIGINT)
        sys_exit(IC_TERMINATE);
    else if (signal == SIGSEGV)
        sys_exit(IC_SEGFAULT);
    else
        sys_exit(IC_SIGNAL);
}

IC_PUBLIC
void sys_setup_signal_handler(void)
{
    signal(SIGILL, sys_signal_handler);
    signal(SIGFPE, sys_signal_handler);
    signal(SIGSEGV, sys_signal_handler);
    signal(SIGTERM, sys_signal_handler);
    signal(SIGINT, sys_signal_handler);
}
