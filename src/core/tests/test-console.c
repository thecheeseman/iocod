#include "ic_test.h"
#include <signal.h>

bool quit = false;
void handle_interrupt(int sig)
{
    UNUSED_PARAM(sig);

    quit = true;
}

int TEST_MAIN()
{
    con_init();

    signal(SIGINT, handle_interrupt);

    con_print("console test\n");
    con_print("^0black text\n");
    con_print("^1red text\n");
    con_print("^2green text\n");
    con_print("^3yellow text\n");
    con_print("^4blue text\n");
    con_print("^5cyan text\n");
    con_print("^6magenta text\n");
    con_print("^7white text\n");
    con_print("^8purple text\n");
    con_print("^9rose text\n");
    con_print("^7to exit, type exit\n");

    while (!quit) {
        char *s = con_input();
        if (s != NULL && strcmp(s, "exit") == 0)
            break;
    }

    con_shutdown();
}
