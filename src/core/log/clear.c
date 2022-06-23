#include "log_local.h"

/*
 * Log clear.
 */
IC_PUBLIC
void log_clear(void)
{
    if (iclog.fp == NULL)
        return;

    iclog.fp = freopen(iclog.path, "wb", iclog.fp);
    if (iclog.fp == NULL) {
        ic_error(_("Couldn't open logfile '%1$s': %2$s\n"),
                 iclog.path, strerror(errno));
        return;
    }

    log_banner();
    log_debug(_("Log cleared\n"));
}
