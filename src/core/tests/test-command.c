#include "ic_test.h"

void echo(struct cmd *self)
{
    UNUSED_PARAM(self);

    con_print(va("%s\n", cmd_args()));
}

void quit(struct cmd *self)
{
    UNUSED_PARAM(self);
    con_print("see ya\n");

    sys_quit();
}

void set_cv(struct cmd *self)
{
    UNUSED_PARAM(self);
    
    uint32_t argc = cmd_argc();
    if (argc < 2) {
        con_print("usage: cv_set <name> <value>\n");
        return;
    }

    if (argc == 2) {
        struct cvar *cv = cv_find(cmd_argv(1));
        if (cv != NULL)
            con_print(va("\"%s\" is: \"%s^7\"", cv->name, cv->string));

        return;
    }

    cv_set2(cmd_argv(1), cmd_args_from(2), false);
}

int TEST_MAIN()
{
    log_init();
    log_set_level(LOG_LEVEL_ALL);
    con_init();
    ev_init();
    cv_init();
    cbuf_init();
    cmd_init();

    cmd_add("echo", echo);
    cmd_add("echo2", echo);
    cmd_add("quit", quit);
    cmd_add("set", set_cv);
    cmd_remove("echo2");

    while (true) {
        char *s = con_input();
        if (s != NULL)
            cbuf_execute_text(EXEC_NOW, s);
    }

    con_shutdown();

    return 0;
}
