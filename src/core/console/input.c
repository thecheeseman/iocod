#include "con_local.h"

#ifdef IC_PLATFORM_WINDOWS
IC_PUBLIC
char *con_input(void)
{
    DWORD events = 0;

    if (!GetNumberOfConsoleInputEvents(console.hin, &events))
        return NULL;

    if (events < 1)
        return NULL;

    INPUT_RECORD buf[MAX_EDIT_LINE];
    if (events >= MAX_EDIT_LINE) {
        ReadConsoleInput(console.hin, buf, 1, &events);
        return NULL;
    }

    DWORD count = 0;
    if (!ReadConsoleInput(console.hin, buf, events, &count))
        return NULL;

    FlushConsoleInputBuffer(console.hin);

    int newline = -1;
    for (DWORD i = 0; i < count; i++) {
        // skip non-key events and key ups
        if (buf[i].EventType != KEY_EVENT || !buf[i].Event.KeyEvent.bKeyDown)
            continue;

        WORD key = buf[i].Event.KeyEvent.wVirtualKeyCode;

        bool breakout = true;
        switch (key) {
        case VK_RETURN:
            newline = i;
            console.cursor = 0;
            break;
        case VK_UP:
        case VK_DOWN:
            // TODO: history
            break;
        case VK_LEFT:
            console.cursor--;

            if (console.cursor < 0)
                console.cursor = 0;
            break;
        case VK_RIGHT:
            console.cursor++;

            if (console.cursor > console.line_len)
                console.cursor = console.line_len;
            break;
        case VK_HOME:
            console.cursor = 0;
            break;
        case VK_END:
            console.cursor = console.line_len;
            break;
        case VK_BACK:
            if (console.cursor > 0) {
                int newlen = console.line_len > 0 ?
                    console.line_len - 1 : 0;

                if (console.cursor < console.line_len) {
                    memmove(console.line + console.cursor - 1,
                            console.line + console.cursor,
                            console.line_len - console.cursor);
                }

                console.line[newlen] = '\0';
                console.line_len = newlen;
                console.cursor--;
            }
            break;
        case VK_DELETE:
            if (console.cursor < console.line_len) {
                int newlen = console.line_len > 0 ?
                    console.line_len - 1 : 0;

                memmove(console.line + console.cursor,
                        console.line + console.cursor + 1,
                        console.line_len - console.cursor);

                console.line_len = newlen;
            }
            break;
        case VK_TAB:
            // TODO: autocomplete
            break;
        default:
            breakout = false;
            break;
        }

        if (breakout)
            break;

        if (console.line_len >= sizeof(console.line) - 1)
            continue; // too many chars

        // .AsciiChar only contains printable chars, everything else is 0
        char c = buf[i].Event.KeyEvent.uChar.AsciiChar;
        if (c == 0)
            continue;

        if (console.line_len > console.cursor) {
            memmove(console.line + console.cursor + 1,
                    console.line + console.cursor,
                    console.line_len - console.cursor);
        }

        console.line[console.cursor++] = c;
        console.line_len++;
        console.line[console.line_len] = '\0';
    }

    if (newline < 0) {
        con_show();
        return NULL;
    }

    if (console.line_len == 0) {
        con_show();
        con_print(IC_PLATFORM_NEWLINE);
        return NULL;
    }

    console.line_len = 0;
    con_show();
    // history_add();

    con_print(va("%s%s", console.line, IC_PLATFORM_NEWLINE));
    return console.line;
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

    static char text[MAX_EDIT_LINE];
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

        write(STDOUT_FILENO, IC_PLATFORM_NEWLINE, strlen(IC_PLATFORM_NEWLINE));
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
