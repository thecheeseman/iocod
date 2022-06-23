#include "log_local.h"

/*
 * Print a banner.
 */
IC_PUBLIC
void log_banner(void)
{
    bool oldstdout = iclog.echo_stdout;
    iclog.echo_stdout = false;

    iclog.hide_next_source = true;
    log_print("--------------------------------------------------------------------------------");

    iclog.echo_stdout = oldstdout;
}
