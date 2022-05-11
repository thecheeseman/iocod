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
 * @file preload.c
 * @date 2022-02-19
 * 
 * This file is for the creation of the LD_PRELOAD hook (based on CoDExtended)
 * which we can use to test the game dll against the original code. It also
 * enables stacktracing and a number of other useful features for debugging
 * the original code.
*/

#include "preload.h"

#define DECLARE(n, o) n##_t n = (n##_t) o;
DECLARE(_cmd_add_command,				0x805aef8)
DECLARE(_com_quit,						0x806d910)
DECLARE(_sys_console_input_shutdown,	0x80c6ad8)
#undef DECLARE

static void cmd_exit(void)
{
    _com_quit();
}

/**
 * Call location from given offset.
 *
 * @author php (riicchhaarrdd)
 * @param off offset to start
 * @param loc location to call
 */
void __call(int off, int loc)
{
    int foffset;

    mprotect((void *) off, 5, PROT_READ | PROT_WRITE | PROT_EXEC);
    foffset = loc - (off + 5);
    memcpy((void *) (off + 1), &foffset, 4);
}

/**
 * @brief 
 * @param argc 
 * @param argv 
 * @return 
*/
int preload_lib_main(int argc, char *argv[])
{
    return ((int(*)(int, char **)) 0x80c6870)(argc, argv); /* call original */
}

/**
 * @brief 
 * @param  
*/
void __attribute__((constructor)) preload_lib_entry(void)
{
    static bool preload_lib_loaded = false;
    if (preload_lib_loaded)
        return;
    preload_lib_loaded = true;

    printf("--------------------\n");
    printf("devpreload.so loaded\n");
    printf("--------------------\n");

    unsetenv("LD_PRELOAD");
    setbuf(stdout, NULL);

    backtrace_init(BT_STD);

    mprotect((void *) 0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
    *(intptr_t *) 0x804a698 = (int) preload_lib_main;

    __call(0x806b8ce, (int) preload_lib_exit);
    *(intptr_t *) 0x806cb0d = (int) preload_lib_exit;

    _cmd_add_command("exit", cmd_exit);
}

/**
 * @brief 
 * @param  
*/
void __attribute__((destructor)) preload_lib_exit(void)
{
    static bool preload_lib_freed = false;
    if (preload_lib_freed)
        return;
    preload_lib_freed = true;

    _com_quit();
}
