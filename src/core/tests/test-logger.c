#include "ic_test.h"

int TEST_MAIN()
{
    con_init();
    log_init();
    log_echo_stdout(true);
    log_set_level(LOG_LEVEL_ALL);

    log_print("example log print messages:");
    log_trace("example trace message");
    log_debug("this is a debug message");
    log_info("standard message");
    log_warn("something isn't going right");
    log_error("we encountered an error");
    log_fatal("unrecoverable");
    log_print("\n");

    log_shutdown();

    return 0;
}
