#include "con_local.h"

#ifdef IC_PLATFORM_WINDOWS
char *con_input(void)
{
    return NULL;
}
#else
// non-interactive/dumb terminals
static char *get_stdin(void)
{
    fd_set fdset;

    FD_ZERO(&fdset);
    FD_SET(STDIN_FILENO, &fdset);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    if (select(STDIN_FILENO + 1, &fdset, NULL, NULL, &timeout) == -1 ||
        !FD_ISSET(STDIN_FILENO, &fdset)) {
        return NULL;
    }

    char text[MAX_EDIT_LINE];
    ssize_t len = read(STDIN_FILENO, text, sizeof(text));
    if (len == 0) {
        // eof
        console.stdin_active = false;
        return NULL;
    }

    if (len < 1)
        return NULL;

    text[len - 1] = 0;
    return text;
}

IC_PUBLIC
char *con_input(void)
{
    static char text[MAX_EDIT_LINE];

    // nothing to do
    if (!console.on && !console.stdin_active)
        return NULL;

    // non-interactive
    if (console.stdin_active)
        return get_stdin();

    char key;
    ssize_t avail = read(STDIN_FILENO, &key, 1);
    if (avail == -1 || key == 0)
        return NULL;

    // handle backspaces
    if ((key == console.tty_erase) || (key == 127) || (key == 8)) {
        if (console.field.cursor > 0) {
            console.field.cursor--;
            console.field.buffer[console.field.cursor] = '\0';
            con_back();
        }

        return NULL;
    }

    // regular chars
    if ((key) >= ' ') {
        // too much data
        if (console.field.cursor >= sizeof(text) - 1)
            return NULL;

        console.field.buffer[console.field.cursor] = key;
        console.field.cursor++;

        write(STDOUT_FILENO, &key, 1);
        return NULL;
    } 

    // handle control chars
    // new lines
    if (key == '\n') {
        history_add(&console.field);
        strncpyz(text, console.field.buffer, sizeof(text));
        memset(&console.field, 0, sizeof(console.field));

        key = '\n';

        write(STDOUT_FILENO, &key, 1);
        write(STDOUT_FILENO, TTY_PROMPT, strlen(TTY_PROMPT));

        return text;
    }

    // autocomplete
    if (key == '\t') {
        con_hide();
        // history_autocomplete(&console.field.buffer);
        con_show();
        return NULL;
    }

    avail = read(STDIN_FILENO, &key, 1);
    if (avail == -1) {
        ic_printf(_("Dropping ISCTL sequence: %d, tty_erase: %d\n"), key,
                    console.tty_erase);
        goto out;
    }

    if (key == '[' || key == 'O') {
        avail = read(STDIN_FILENO, &key, 1);
        if (avail == -1) {
            ic_printf(_("Dropping ISCTL sequence: %d, tty_erase: %d\n"), key,
                        console.tty_erase);
            goto out;
        }

        struct field *history;
        switch (key) {
        case 'A':
            history = history_previous();
            if (history != NULL) {
                con_hide();
                console.field = *history;
                con_show();
            }

            break;
        case 'B':
            history = history_next();
            con_hide();

            if (history != NULL)
                console.field = *history;
            else
                memset(&console.field, 0, sizeof(console.field));

            con_show();
            break;
        case 'C':
        case 'D':
            break;
        }
    }

out:
    tcflush(STDIN_FILENO, TCIFLUSH);
    return NULL;
}
#endif
