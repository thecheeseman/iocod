/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

#include "shared.h"
#include "cbuf.h"
#include "common.h"
#include "common/error.h"
#include "common/print.h"
#include "types/byte.h"

#define MAX_CMD_BUFFER 16384
#define MAX_CMD_LINE 1024

struct cmd {
    byte *data;
    int maxsize;
    int cursize;
};

static int cmd_wait;
static struct cmd cmd_text;
static byte cmd_text_buf[MAX_CMD_BUFFER];

/**
 * @brief Initialize command buffer
*/
void cbuf_init(void)
{
    cmd_text.data = cmd_text_buf;
    cmd_text.maxsize = MAX_CMD_BUFFER;
    cmd_text.cursize = 0;
}

/**
 * @brief Appends command text at the end of the buffer, without trailing \n
 *
 * @param text
*/
void cbuf_add_text(const char *text)
{
    int l = strlen(text);

    if (cmd_text.cursize + l >= cmd_text.maxsize) {
        com_printf("cbuf_add_text: overflow\n");
        return;
    }

    memcpy(&cmd_text.data[cmd_text.cursize], text, l);
    cmd_text.cursize += l;
}

/**
 * @brief Inserts command text immediately after the current command,
 * with trailing \n
 *
 * @param text
*/
void cbuf_insert_text(const char *text)
{
    int i, len = strlen(text) + 1;

    if ((len + cmd_text.cursize) > cmd_text.maxsize) {
        com_printf("cbuf_insert_text: overflow\n");
        return;
    }

    // move the existing command text
    for (i = cmd_text.cursize - 1; i >= 0; i--)
        cmd_text.data[i + len] = cmd_text.data[i];

    memcpy(cmd_text.data, text, len - 1);
    cmd_text.data[len - 1] = '\n';
    cmd_text.cursize += len;
}

/**
 * @brief Insert, append or immediately execute command buffer text
 * @param type
 * @param text
*/
void cbuf_execute_text(enum cbuf_exec_type type, const char *text)
{
    switch (type) {
        case EXEC_NOW:
            if (text != NULL && strlen(text) > 0)
                cmd_execute_string(text);
            else
                cbuf_execute();
            break;
        case EXEC_INSERT:
            cbuf_insert_text(text);
            break;
        case EXEC_APPEND:
            cbuf_add_text(text);
            break;
        default:
            com_error(ERR_FATAL, "cbuf_execute_text: bad when");
    }
}

/**
 * @brief Execute command buffer
*/
void cbuf_execute(void)
{
    int i, quotes;
    char *text, line[MAX_CMD_LINE];

    while (cmd_text.cursize) {
        if (cmd_wait) {
            // skip out while text still remains in buffer, leaving it
            // for next frame
            cmd_wait--;
            break;
        }

        // find a \n or ; line break
        text = (char *) cmd_text.data;

        quotes = 0;
        for (i = 0; i < cmd_text.cursize; i++) {
            if (text[i] == '"')
                quotes++;

            if (!(quotes & 1) && text[i] == ';')
                break;  // don't break if inside a quoted string

            if (text[i] == '\n' || text[i] == '\r')
                break;
        }

        if (i >= (MAX_CMD_LINE - 1))
            i = MAX_CMD_LINE - 1;

        memcpy(line, text, i);
        line[i] = 0;

        // delete the text from the command buffer and move remaining commands down
        // this is necessary because commands (exec) can insert data at the
        // beginning of the text buffer
        if (i == cmd_text.cursize) {
            cmd_text.cursize = 0;
        } else {
            i++;
            cmd_text.cursize -= i;
            memmove(text, text + i, cmd_text.cursize);
        }

        // execute the command line
        cmd_execute_string(line);
    }
}
