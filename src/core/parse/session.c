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

#include "iocod.h"

#define MAX_PARSE_INFO 16
static struct parse_info info[MAX_PARSE_INFO];
static size_t info_num = 0;
struct parse_info *pi = &info[0];

IC_PUBLIC
IC_NON_NULL(1)
void ps_begin_session(const char *name)
{
    if (info_num == MAX_PARSE_INFO - 1) {
        ic_error("session overflow\n");
        return;
    }

    info_num++;
    pi = &info[info_num];

    pi->lines = 1;
    pi->unget_token = false;

    strncpyz(pi->parse_file, name, sizeof(pi->parse_file));
}

IC_PUBLIC
void ps_end_session(void)
{
    if (info_num == 0) {
        ic_error("session underflow\n");
        return;
    }

    info_num--;
    pi = &info[info_num];
}

IC_PUBLIC
void ps_reset(void)
{
    pi = &info[0];
    info_num = 0;
}
