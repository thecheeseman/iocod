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

#include "log_local.h"

struct logger iclog = {
    .name = "iocod",
    .path = "iocod.log",
    .level = LOG_LEVEL_INFO,
    .echo_stdout = true,
    .auto_lf = true,
    .hide_next_source = false,
    .size = 0
};

#ifdef IC_PLATFORM_WINDOWS
CRITICAL_SECTION log_mutex;
#else
pthread_mutex_t log_mutex;
#endif

/*
 * Log init.
 */
IC_PUBLIC
void log_init(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    InitializeCriticalSection(&log_mutex);
    #else
    if (pthread_mutex_init(&log_mutex, NULL) != 0) {
        ic_error(_("Mutex init failed\n"));
        return;
    }
    #endif

    iclog.fp = fopen(iclog.path, "ab");
    if (iclog.fp == NULL) {
        ic_error(_("Couldn't open logfile '%1$s': %2$s\n"), 
                 iclog.path, strerror(errno));
        return;
    }

    /* get current file size */
    ssize_t pos = ftell(iclog.fp);
    fseek(iclog.fp, 0, SEEK_END);
    ssize_t end = ftell(iclog.fp);
    fseek(iclog.fp, pos, SEEK_SET);
    iclog.size = end;

    /* TODO: file splitting once file gets too large? */

    log_banner();
    log_debug(_("Log file opened\n"));
}

/*
 * Log shutdown.
 */
IC_PUBLIC
void log_shutdown(void)
{
    log_debug(_("Log file closed\n"));
    log_banner();

    fclose(iclog.fp);
    iclog.fp = NULL;

    #ifdef IC_PLATFORM_WINDOWS
    DeleteCriticalSection(&log_mutex);
    #else
    pthread_mutex_destroy(&log_mutex);
    #endif
}

/*
 * Set log stdout.
 */
IC_PUBLIC
void log_echo_stdout(bool echo)
{
    IC_ASSERT((echo == true) || (echo == false));

    if (iclog.echo_stdout != echo) {
        log_debug(_("log option 'echo_stdout' changed to '%s'\n"),
                  echo ? _("true") : _("false"));

        iclog.echo_stdout = echo;
    }
}

/*
 * Set auto lf.
 */
IC_PUBLIC
void log_auto_lf(bool lf)
{
    IC_ASSERT((lf == true) || (lf == false));

    if (iclog.auto_lf != lf) {
        log_debug(_("log option 'auto_lf' changed to '%s'\n"),
                  lf ? _("true") : _("false"));

        iclog.auto_lf = lf;
    }
}
