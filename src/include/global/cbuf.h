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
 * @file cbuf.h
 * @date 2022-02-04
*/

#ifndef __CBUF_H__
#define __CBUF_H__

// 
//
// commands/cmd.c
//
//
void cbuf_init(void);
void cbuf_add_text(const char *text);
void cbuf_insert_text(const char *text);
void cbuf_execute_text(enum cbuf_exec_type type, const char *text);
void cbuf_execute(void);

#endif // __CBUF_H__
