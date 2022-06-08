#include "ic_test.h"
#include <signal.h>

extern void con_init(void);
extern void con_shutdown(void);
extern void con_print(const char *msg);
extern char *con_input(void);
void con_set_color(enum q3color fore, enum q3color back);
void con_set_text_color(enum q3color color);
void con_set_back_color(enum q3color color);

bool quit = false;
void handle_interrupt(int sig)
{
    quit = true;
}

int TEST_MAIN()
{
    con_init();

    signal(SIGINT, handle_interrupt);

    con_print("this is a ^1test ^7message ^6with ^5colours\n");
    con_print("asdf\n");

    while (!quit) {
        char *s = con_input();
        if (s != NULL && strcmp(s, "exit") == 0)
            break;
    }

    con_shutdown();
}
