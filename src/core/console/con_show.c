#include "con_local.h"

#ifdef IC_PLATFORM_WINDOWS
void con_back(void)
{

}

void con_hide(void)
{

}

void con_show(void)
{

}
#else
void con_back(void)
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
    if (!console.on)
        return;

    if (console.hide > 0) {
        console.hide++;
        return;
    }

    if (console.field.cursor > 0) {
        for (int i = 0; i < console.field.cursor; i++)
            con_back();
    }

    // delete prompt
    for (int i = strlen(TTY_PROMPT); i > 0; i--)
        con_back();

    console.hide++;
}

void con_show(void)
{
    if (!console.on)
        return;

    assert(console.hide > 0);
    console.hide--;

    if (console.hide == 0) {
        write(STDOUT_FILENO, TTY_PROMPT, strlen(TTY_PROMPT));

        if (console.field.cursor > 0) {
            for (int i = 0; i < console.field.cursor; i++)
                write(STDOUT_FILENO, console.field.buffer + i, 1);
        }
    }
}
#endif
