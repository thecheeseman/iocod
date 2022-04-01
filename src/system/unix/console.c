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

/**
 * @file unix_tty.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

#include "cvar/cvar.h"

#include <termios.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "system/shared.h"
#include "system/unix/console.h"
#include "stringlib.h"

bool stdin_active = true;

static struct cvar *ttycon = NULL;
bool ttycon_on = false;
int ttycon_hide = 0;

static int tty_erase;
static int tty_eof;
static struct termios tty_tc;

static struct field tty_con;

struct cvar *tty_colors;

// history
#define TTY_HISTORY 32
static struct field tty_edit_lines[TTY_HISTORY];
static int hist_current = -1, hist_count = 0;

void console_input_shutdown(void)
{
    if (ttycon_on != NULL) {
        fprintf(stdout, "Shutdown tty console\n");
        tcsetattr(0, TCSADRAIN, &tty_tc);
    }
}

void console_input_init(void)
{
    struct termios tc;

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    ttycon = cvar_get("ttycon", "1", 0);
    if (ttycon != NULL && ttycon->value != 0) {
        if (isatty(STDIN_FILENO) != 1) {
            printf("stdin is not a tty, tty console mode failed\n");
            cvar_set("ttycon", "0");
            ttycon_on = false;
            return;
        }

        printf("started tty console (use +set ttycon 0 to disable)\n");
        field_clear(&tty_con);
        tcgetattr(0, &tty_tc);
        tty_erase = tc.c_cc[VERASE];
        tty_eof = tc.c_cc[VEOF];
        tc = tty_tc;
        /*
         ECHO: don't echo input characters
         ICANON: enable canonical mode.  This  enables  the  special
                  characters  EOF,  EOL,  EOL2, ERASE, KILL, REPRINT,
                  STATUS, and WERASE, and buffers by lines.
         ISIG: when any of the characters  INTR,  QUIT,  SUSP,  or
                  DSUSP are received, generate the corresponding sig­
                  nal
        */
        tc.c_lflag &= ~(ECHO | ICANON);
        /*
         ISTRIP strip off bit 8
         INPCK enable input parity checking
         */
        tc.c_iflag &= ~(ISTRIP | INPCK);
        tc.c_cc[VMIN] = 1;
        tc.c_cc[VTIME] = 0;
        tcsetattr(0, TCSADRAIN, &tc);
        ttycon_on = true;
    } else {
        ttycon_on = false;
    }
}

/*
 * Flush stdin
 */
void tty_flush_in(void)
{
    char key;
    while (read(0, &key, 1) != -1);
}

/*
 * Do a backspace
 */
void tty_back(void)
{
    char key;
    key = '\b';
    write(1, &key, 1);
    key = ' ';
    write(1, &key, 1);
    key = '\b';
    write(1, &key, 1);
}

/*
 * Clear the display of the line currently edited
 * Bring cursor back to the beginning of the line
 */
void tty_hide(void)
{
    int i;

    assert(ttycon_on);

    if (ttycon_hide) {
        ttycon_hide++;
        return;
    }

    if (tty_con.cursor > 0) {
        for (i = 0; i < tty_con.cursor; i++)
            tty_back();
    }

    ttycon_hide++;
}

/*
 * Show the current line
 */
void tty_show(void)
{
    int i;

    assert(ttycon_on);
    assert(ttycon_hide > 0);

    ttycon_hide--;

    if (ttycon_hide == 0) {
        if (tty_con.cursor) {
            for (i = 0; i < tty_con.cursor; i++)
                write(1, tty_con.buffer + i, 1);
        }
    }
}

void hist_add(struct field *field)
{
    int i;

    assert(hist_count <= TTY_HISTORY);
    assert(hist_count >= 0);
    assert(hist_current >= -1);
    assert(hist_current <= hist_count);

    // make some room
    for (i = TTY_HISTORY - 1; i > 0; i--)
        tty_edit_lines[i] = tty_edit_lines[i - 1];

    tty_edit_lines[0] = *field;

    if (hist_count < TTY_HISTORY)
        hist_count++;

    hist_current = -1; // re-init
}

struct field *hist_prev(void) 
{
    int hist_prev;

    assert(hist_count <= TTY_HISTORY);
    assert(hist_count >= 0);
    assert(hist_current >= -1);
    assert(hist_current <= hist_count);

    hist_prev = hist_current + 1;
    if (hist_prev >= hist_count) 
        return NULL;

    hist_current++;
    return &(tty_edit_lines[hist_current]);
}

struct field *hist_next(void) {
    assert(hist_count <= TTY_HISTORY);
    assert(hist_count >= 0);
    assert(hist_current >= -1);
    assert(hist_current <= hist_count);

    if (hist_current >= 0)
        hist_current--;

    if (hist_current == -1)
        return NULL;

    return &(tty_edit_lines[hist_current]);
}

/*
 * 
 */
char *console_input(void)
{
    static char text[256];
    int i, avail;
    char key;
    struct field *history;

    if (ttycon && ttycon->value) {
        avail = read(0, &key, 1);

        // we have something
        if (avail != -1) {
            // backspace
            if ((key == tty_erase) || (key == 127) || (key == 8)) {
                if (tty_con.cursor > 0) {
                    tty_con.cursor--;
                    tty_con.buffer[tty_con.cursor] = '\0';
                    tty_back();
                }

                return NULL;
            }

            // control char
            if ((key) && (key) < ' ') {
                // new line
                if (key == '\n') {
                    // push into history
                    hist_add(&tty_con);
                    strcpy(text, tty_con.buffer);
                    field_clear(&tty_con);
                    key = '\n';
                    write(1, &key, 1);
                    return text;
                }

                // tab autocompletion
                if (key == '\t') {
                    tty_hide();
                    field_complete_command(&tty_con);

                    // clean up field_complete_command
                    tty_con.cursor = strlen(tty_con.buffer);
                    if (tty_con.cursor > 0) {
                        if (tty_con.buffer[0] == '\\') {
                            for (i = 0; i <= tty_con.cursor; i++)
                                tty_con.buffer[i] = tty_con.buffer[i + 1];

                            tty_con.cursor--;
                        }
                    }

                    tty_show();
                    return NULL;
                }

                avail = read(0, &key, 1);
                if (avail != -1) {
                    // vt 100 keys
                    if (key == '[' || key == 'O') {
                        avail = read(0, &key, 1);
                        if (avail != -1) {
                            switch (key) {
                                case 'A':
                                    history = hist_prev();
                                    if (history) {
                                        tty_hide();
                                        tty_con = *history;
                                        tty_show();
                                    }

                                    tty_flush_in();
                                    return NULL;
                                    break;
                                case 'B':
                                    history = hist_next();

                                    tty_hide();
                                    if (history)
                                        tty_con = *history;
                                    else
                                        field_clear(&tty_con);
                                    tty_show();

                                    tty_flush_in();
                                    return NULL;
                                    break;
                                case 'C':
                                case 'D':
                                    return NULL;
                                    break;
                            }
                        }
                    }
                }

                printf("dropping ISCTL sequence: %d, tty_erase: %d\n", key, 
                       tty_erase);
                tty_flush_in();
                return NULL;
            }

            // push regular character
            tty_con.buffer[tty_con.cursor] = key;
            tty_con.cursor++;

            // print the current line
            write(1, &key, 1);
        }

        return NULL;
    } else {
        int len;
        fd_set fdset;
        struct timeval timeout;

        if (!stdin_active)
            return NULL;

        FD_ZERO(&fdset);
        FD_SET(0, &fdset);
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        if (select(1, &fdset, NULL, NULL, &timeout) == -1 || 
            !FD_ISSET(0, &fdset))
            return NULL;

        len = read(0, text, sizeof(text));
        if (len == 0) { // eof
            stdin_active = false;
            return NULL;
        }

        if (len < -1)
            return NULL;

        text[len - 1] = 0; // remove /n 
        return text;
    }
}

/**
 * @brief Print a message to the console
 * @param msg 
*/
void sys_print(const char *msg)
{
    if (ttycon_on)
        tty_hide();

    fputs(msg, stderr);

    if (ttycon_on)
        tty_show();
}
