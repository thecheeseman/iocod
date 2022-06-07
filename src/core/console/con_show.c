#include "con_local.h"

#ifdef IC_PLATFORM_WINDOWS

#else
static void con_back(void)
{
    char key = '\b';
    write(STDOUT_FILENO, &key, 1);
    key = ' ';
    write(STDOUT_FILENO, &key, 1);
    key = '\b';
    write(STDOUT_FILENO, &key, 1);
}

void con_hide(void)
{
    if (!condata.on)
        return;

    if (condata.hide > 0) {
        condata.hide++;
        return;
    }

    if (condata.con.cursor > 0) {
        for (int i = 0; i < condata.con.cursor; i++)
            con_back();
    }

    // delete prompt
    for (int i = strlen(TTY_PROMPT); i > 0; i--)
        con_back();

    condata.hide++;
}

void con_show(void)
{
    if (!condata.on)
        return;

    assert(condata.hide > 0);
    condata.hide--;

    if (condata.hide == 0) {
        write(STDOUT_FILENO, TTY_PROMPT, strlen(TTY_PROMPT));

        if (condata.con.cursor > 0) {
            for (int i = 0; i < condata.con.cursor; i++)
                write(STDOUT_FILENO, condata.con.buffer + i, 1);
        }
    }
}
#endif
