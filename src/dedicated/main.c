#include "iocod.h"
#include <signal.h>
#include <float.h>

static void print_gpl(void)
{
    con_print("iocod copyright (C) 2022 thecheeseman\n"
              "iocod comes with ABSOLUTELY NO WARRANTY; for details use the "
              "command 'gplinfo'.\nThis is free software, and you are welcome "
              "to redistribute it under certain\nconditions; use 'gplinfo' "
              "for details.\n\n");
}

IC_PUBLIC
int main(int argc, char *argv[])
{
    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    con_init();
    con_set_title("iocod dedicated server console "
                  "[" IC_VERSION_STRING "-" IC_PLATFORM_STRING "]");

    print_gpl();

    config_init();
    log_init();

    // autoupdate?

    sys_setup_signal_handler();
    sys_set_floatenv();
    sys_milliseconds();

    // parse args
    // argc/argv
    
    // com_init
    net_init();
    
    while (true) {
        char *s = con_input();
        if (s != NULL && strcmp(s, "exit") == 0)
            break;
    }

    sys_quit();
    return 0;
}
