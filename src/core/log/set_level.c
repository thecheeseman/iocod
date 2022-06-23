#include "log_local.h"

/* string representation of log level */
static const char *level_str[] = {
    [LOG_LEVEL_NONE] = "none",
    [LOG_LEVEL_FATAL] = "fatal",
    [LOG_LEVEL_ERROR] = "error",
    [LOG_LEVEL_WARN] = "warn",
    [LOG_LEVEL_INFO] = "info",
    [LOG_LEVEL_DEBUG] = "debug",
    [LOG_LEVEL_TRACE] = "trace",
    [LOG_LEVEL_ALL] = "all"
};

/*
 * Set log level.
 */
IC_PUBLIC
void log_set_level(enum log_level new_level)
{
    IC_ASSERT((new_level >= LOG_LEVEL_NONE) && (new_level <= LOG_LEVEL_ALL));

    if (iclog.level != new_level) {
        log_debug(_("Log level changed from '%1$s' to '%2$s'\n"),
                  level_str[iclog.level], level_str[new_level]);

        iclog.level = new_level;
    }
}
