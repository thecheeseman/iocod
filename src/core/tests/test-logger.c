#include "iocod.h"

int main(void)
{
    log_init();
    log_set_level(LOG_LEVEL_ALL);

    log_print("this is a test");
    log_print("this is another test with a new line\n");

    log_info("this is an info message");

    log_warn("here's a warning");

    log_error("oh no");

    log_fatal("FATAL ERROR");

    log_trace("this is a really long line log_banner();log_banner();log_banner();"\
              "log_banner(); log_banner(); log_banner(); log_banner(); log_banner();"\
              "log_banner(); so yeah");

    log_shutdown();
}
