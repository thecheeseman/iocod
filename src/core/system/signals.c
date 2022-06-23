#include "iocod.h"
#include <signal.h>

IC_PUBLIC
void sys_signal_handler(int signal)
{
    static bool caught = false;
    static bool caught_sigint = false;
    static int32_t last_sigint = 0;

    if (signal == SIGINT) {
        if (!caught_sigint || sys_milliseconds() - last_sigint > 3000) {
            caught_sigint = true;
            last_sigint = sys_milliseconds();

            con_print(_("Press Ctrl-C again within 3 seconds to exit\n"));
            return;
        }

        if (sys_milliseconds() - last_sigint < 3000)
            goto quit;
    } else {
        if (caught) {
            con_print(va(_("Double signal fault: received %d, exiting...\n"), 
                         signal));
        } else {
            caught = true;

            // cl shutdown
            // sv shutdown
        }
    }

quit:
    con_print(va(_("Signal %d caught, terminating...\n"), signal));
    
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
