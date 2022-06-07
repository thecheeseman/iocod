#include "ic_test.h"
#include <signal.h>

extern void con_init();
extern void con_shutdown();

bool quit = false;
void handle_interrupt(int sig)
{
    quit = true;
}

int TEST_MAIN()
{
    con_init();

    signal(SIGINT, handle_interrupt);

    while (!quit) {
        ; // do nothing
    }

    con_shutdown();
}
