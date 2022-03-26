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
 * @file unix_system.c
 * @date 2022-02-04
*/

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fpu_control.h>
#include <errno.h>
#include <signal.h>
#include <dlfcn.h>

#include "shared.h"
#include "common.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "stringlib.h"

#define MAX_CMD 1024
static char exit_cmdline[MAX_CMD] = "";

void sys_do_start_process(char *cmdline)
{
    switch (fork()) {
        case -1:
            // main thread
            break;
        case 0:
            if (strchr(cmdline, ' ')) {
                system(cmdline);
            } else {
                execl(cmdline, cmdline, NULL);
                printf("execl failed: %s\n", strerror(errno));
            }
            _exit(0);
            break;
    }
}

void sys_exit(int ex)
{
    sys_console_input_shutdown();

    if (exit_cmdline[0] != '\0') {
        sleep(1);
        sys_do_start_process(exit_cmdline);
        sleep(1);
    }

    assert(ex == 0);
    _exit(ex);
}

void sys_quit(void)
{
    fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) * ~FNDELAY);
    sys_exit(0);
}

void sys_init(void)
{
    cvar_set("arch", PLATFORM_STRING);
    cvar_set("username", sys_get_current_user());
}

/**
 * Simple check for either --version or -v as passed arguments
 * Originally Sys_ParseArgs()
 */
void sys_check_for_version(int argc, char *argv[])
{
    if (argc == 2) {
        if ((!strcmp(argv[1], "--version")) ||
            (!strcmp(argv[1], "-v"))) {

            // collapsed from Sys_PrintBinVersion
            fprintf(stdout, "\n\n==============================================================\n");
            fprintf(stdout, "iocod Dedicated Server [%s %s]\n", __DATE__, __TIME__);
            fprintf(stdout, " local install: %s\n", argv[0]);
            fprintf(stdout, "==============================================================\n\n");
            // collapsed from Sys_PrintBinVersion

            sys_exit(0);
        }
    }
}

void sys_configure_fpu(void)
{
    #ifdef DEBUG
    static int fpu_word = _FPU_DEFAULT & ~(_FPU_MASK_ZM | _FPU_MASK_IM);
    int current = 0;
    _FPU_GETCW(current);

    if (current != fpu_word) {
        fprintf(stderr, "fpu control 0x%x (was 0x%x)\n", fpu_word, current);
        _FPU_SETCW(fpuword);
        _FPU_GETCW(current);
        assert(fpu_word == current);
    }
    #else
    static int fpu_word = _FPU_DEFAULT;
    _FPU_SETCW(fpu_word);
    #endif
}

/**
 * @brief Print an error message
 * @param error The error message format
*/
void sys_error(const char *error, ...)
{
    va_list argptr;
    char string[1024];

    fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) & ~FNDELAY);

    if (ttycon_on)
        tty_hide();

    va_start(argptr, error);
    vsprintf(string, error, argptr);
    va_end(argptr);

    if (tty_colors->integer)
        fprintf(stderr, "\n\033[31m**** Error: \033[0m%s \033[31m****\033[0m\n", 
                string);
    else
        fprintf(stderr, "\n**** Error: %s ****\n", string);

    sys_exit(0);
}

void sys_warn(char *warning, ...)
{
    va_list argptr;
    char string[1024];

    va_start(argptr, warning);
    vsprintf(string, warning, argptr);
    va_end(argptr);

    if (ttycon_on)
        tty_hide();

    if (tty_colors->integer)
        fprintf(stderr, "\033[33m**** Warning: \033[0m%s \033[33m****\033[0m\n", 
                string);
    else
        fprintf(stderr, "**** Warning: %s ****\n", string);

    if (ttycon_on)
        tty_show();
}

void sys_show_console(int viewlog, int b)
{
    UNUSED(viewlog);
    UNUSED(b);
}

void *sys_load_dll(const char *name, char *fqpath, 
    intptr_t (**entrypoint)(intptr_t, ...), intptr_t (*systemcalls)(intptr_t, ...))
{
    void *libhandle;
    void (*dllentry)(intptr_t (*syscallptr)(intptr_t, ...));
    char fname[MAX_OSPATH];
    char /**pwdpath,*/ *homepath, *basepath, *gamedir;
    char fn[MAX_OSPATH];
    const char *err = NULL;

    *fqpath = 0;

    snprintf(fname, MAX_OSPATH, "%s.mp.%s.%s", name, ARCH_STRING, DLL_EXT);

    //pwdpath = sys_cwd();
    homepath = cvar_variable_string("fs_homepath");
    basepath = cvar_variable_string("fs_basepath");
    gamedir = cvar_variable_string("fs_game");

    //
    fs_build_ospath(homepath, gamedir, fname, fn);
    com_printf("sys_load_dll(%s)... ", fn);
    libhandle = dlopen(fn, RTLD_NOW);

    if (!libhandle) {
        com_printf("failed:\n  %s\n", dlerror());

        // basepath
        fs_build_ospath(basepath, gamedir, fname, fn);
        com_printf("sys_load_dll(%s)... ", fn);
        libhandle = dlopen(fn, RTLD_NOW);

        // not found, bail
        if (!libhandle) {
            com_printf("failed:\n  %s\n", dlerror());
            return NULL;
        } else {
            com_printf("ok\n");
        }
    } else {
        com_printf("ok\n");
    }

    // ISO C forbids assignment between function pointer and `void *`
    // dllentry = dlsym(libhandle, "dllEntry");
    *(void **) (&dllentry) = dlsym(libhandle, "dllEntry");
    // is this more dangerous than just disabling -pedantic?

    // ISO C forbids assignment between function pointer and `void *`
    // *entrypoint = dlsym(libhandle, "vmMain");
    *(void **)*(&entrypoint) = dlsym(libhandle, "vmMain");
    // is this more dangerous than just disabling -pedantic?

    if (!*entrypoint || !dllentry) {
        com_printf("sys_load_dll(%s) failed dlsym(vmMain:\n\"%s\"!\n", name, 
                   dlerror());
     
        dlclose(libhandle);
        err = dlerror();
        if (err != NULL)
            com_printf("sys_load_dll(%s) failed dlclose:\n\"%s\"\n", name, err);

        return NULL;
    }

    com_printf("sys_load_dll(%s) found **vmMain** at %p\n", name, *entrypoint);
    dllentry(systemcalls);
    com_printf("sys_load_dll(%s) succeeded!\n", name);

    return libhandle;
}

void sys_unload_dll(void *dllhandle)
{
    if (!dllhandle)
        return;

    dlclose(dllhandle);
}
