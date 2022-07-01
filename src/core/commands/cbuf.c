/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include "cmd_local.h"

struct cmdbuf {
    byte *data;
    size_t maxsize;
    size_t cursize;
};

struct cmdbuf cmd_text;
byte cmd_text_buf[MAX_CMD_BUFFER];

IC_PUBLIC
void cbuf_init(void)
{
    cmd_text.data = cmd_text_buf;
    cmd_text.cursize = 0;
    cmd_text.maxsize = MAX_CMD_BUFFER;
}

IC_PUBLIC
bool cbuf_add_text(const char *text)
{
    size_t l = strlen(text);
    if (cmd_text.cursize + l >= cmd_text.maxsize) {
        log_warn(_("cbuf_add_text: overflow"));
        return false;
    }

    memcpy(&cmd_text.data[cmd_text.cursize], text, l);
    cmd_text.cursize += l;

    return true;
}

IC_PUBLIC
bool cbuf_insert_text(const char *text)
{
    size_t len = strlen(text) + 1;
    if (len + cmd_text.cursize > cmd_text.maxsize) {
        log_warn(_("overflow"));
        return false;
    }

    // move the existing command text
    memmove(&cmd_text.data[len], &cmd_text.data[0], cmd_text.cursize);

    // copy the new text in
    memcpy(&cmd_text.data[0], text, len);

    // add a \n
    cmd_text.data[len - 1] = '\n';
    cmd_text.cursize += len;

    return true;
}

IC_PUBLIC
bool cbuf_execute_text(int when, const char *text)
{
    switch (when) {
    case EXEC_NOW:
        if (text != NULL && strlen(text) > 0) {
            log_debug("exec_now %s", text);
            cmd_execute_string(text);
        } else {
            cbuf_execute();
            log_debug("exec now %s", cmd_text.data);
        }
        break;
    case EXEC_INSERT:
        cbuf_insert_text(text);
        break;
    case EXEC_APPEND:
        cbuf_add_text(text);
        break;
    default:
        log_error("bad exec when");
        return false;
    }

    return true;
}

static size_t check_for_breaks(char *text)
{
    size_t i = 0;
    int quotes = 0;

    for (; i < cmd_text.cursize; i++) {
        if (text[i] == '"')
            quotes++;

        if ((quotes & 1) == 0 && text[i] == ';')
            break; // don't break if inside a quoted string

        if (text[i] == '\n' || text[i] == '\r')
            break;
    }

    if (i >= MAX_CMD_LINE - 1)
        i = MAX_CMD_LINE - 1;

    return i;
}

IC_PUBLIC
bool cbuf_execute(void)
{
    while (cmd_text.cursize) {
        if (cmd_wait > 0) {
            // skip out while text still remains in the buffer
            // leave for the next frame
            cmd_wait--;
            break;
        }

        // find a \n or ; line break or comment // or /* */
        char *text = (char *) cmd_text.data;
        size_t len = check_for_breaks(text);

        char line[MAX_CMD_LINE];
        memcpy(line, text, len);
        line[len] = '\0';

        // delete the text from the command buffer and move remaining commands down
        // this is necessary because commands (exec) can insert data at the
        // beginning of the text buffer
        if (len < cmd_text.cursize) {
            len++;
            cmd_text.cursize -= len;
            memmove(text, text + len, cmd_text.cursize);
        } else {
            cmd_text.cursize = 0;
        }

        // execute the command line
        cmd_execute_string(line);
    }

    return true;
}
