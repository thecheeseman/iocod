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

#include "conf_local.h"

static struct conf *icconf = NULL;
static struct confopt opts[] = {
    CONF_HEADER("Config"),

    CONF_SECTION("Config file syntax notes"),

    CONF_COMMENT("Any line that begins with a '#' is treated as a comment."),
    CONF_COMMENT("Variables can be booleans, integers, floats, or strings."),
    CONF_COMMENT("  Booleans"),
    CONF_COMMENT("  - Valid values are: true, false, yes, no, on, off"),
    CONF_COMMENT("  Integers"),
    CONF_COMMENT("  - Can be any regular number (42, 25, etc)"),
    CONF_COMMENT("  - Can also be hexadecimal (0xFE) or octal (031)"),
    CONF_COMMENT("  Floats"),
    CONF_COMMENT("  - Can be any floating number (3.1415, etc)"),
    CONF_COMMENT("  - Can also use exponents (+1.56E4)"),
    CONF_COMMENT("  Strings"),
    CONF_COMMENT("  - Strings must be enclosed in \"quotes\""),
    CONF_BLANK(),

    CONF_HEADER("General"),

    CONF_COMMENT("Console language"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    string"),
    CONF_COMMENT("options: \"english\""),
    CONF_COMMENT("default: \"english\""),
    CONF_STRING("language", "english"),
    CONF_BLANK(),

    CONF_COMMENT("Log level"),
    CONF_COMMENT("Levels are as listed below:"),
    CONF_COMMENT("  0 = No logging"),
    CONF_COMMENT("  1 = Log only fatal errors"),
    CONF_COMMENT("  2 = Log ALL errors"),
    CONF_COMMENT("  3 = Log ALL errors and warnings"),
    CONF_COMMENT("  4 = Log errors, warnings, and useful messages"),
    CONF_COMMENT("  5 = Debug messages"),
    CONF_COMMENT("  6 = Trace messages (very verbose)"),
    CONF_COMMENT("  7 = Log everything"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    integer"),
    CONF_COMMENT("options: 0 - 7"),
    CONF_COMMENT("default: 4 (errors, warnings + useful messages)"),
    #ifdef IC_DEBUG
    CONF_INT("log_level", 7),
    #else
    CONF_INT("log_level", 4),
    #endif
    CONF_BLANK(),

    CONF_COMMENT("Log buffering"),
    CONF_COMMENT("If enabled, log file will only write once the log buffer"),
    CONF_COMMENT("is completely full. You can disable this if you want, and"),
    CONF_COMMENT("the log will always write immediately, but this could"),
    CONF_COMMENT("impact performance. Recommended setting is on."),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: on"),
    CONF_BOOL("log_buffering", on),
    CONF_BLANK(),

    CONF_COMMENT("log buffer size"),
    CONF_COMMENT("If log_buffering is enabled, this is the amount of data"),
    CONF_COMMENT("that will be buffered before the log is written to disk."),
    CONF_COMMENT("You may get increased performance by changing this, but"),
    CONF_COMMENT("it is not guaranteed."),
    CONF_COMMENT(""),
    CONF_COMMENT("Windows defaults to 512."),
    CONF_COMMENT("Linux distros generally are 4096 or 8192 by default."),
    CONF_COMMENT(""),
    CONF_COMMENT("If this is set to 0, then the OS will determine the best"),
    CONF_COMMENT("buffer size (recommended setting)."),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    integer"),
    CONF_COMMENT("options: 0 - 1048576 (1 mb)"),
    CONF_COMMENT("default: 0 (let OS determine buffer size)"),
    CONF_INT("log_buffer_size", 0),
    CONF_BLANK(),

    CONF_HEADER("Compatibility"),

    CONF_COMMENT("Should we use codextended compatible GSC functions?"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: true"),
    CONF_BOOL("codextended_compat", on),
    CONF_BLANK(),

    CONF_COMMENT("Controls which version of cod1 we are targeting"),
    CONF_COMMENT("- To target 1.1, set this to 1"),
    CONF_COMMENT("- To target 1.5, set this to 5"),
    CONF_COMMENT("- To target both, set this to 0 (recommended)"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    integer"),
    CONF_COMMENT("options: 0, 1, 5"),
    CONF_COMMENT("default: 0"),
    CONF_INT("cod_version_target", 0),
    CONF_BLANK(),

    CONF_HEADER("Autoupdate"),

    CONF_SECTION("Note about autoupdates"),
    CONF_COMMENT("Recommended settings for STABLE servers are:"),
    CONF_COMMENT("  autoupdate_enabled on"),
    CONF_COMMENT("  autoupdate_branch \"stable\""),
    CONF_COMMENT("  autoupdate_on_major_release no"),
    CONF_COMMENT("  autoupdate_on_minor_release yes"),
    CONF_COMMENT("  autoupdate_on_patch_release no"),
    CONF_COMMENT(""),
    CONF_COMMENT("The STABLE settings will only update when there are"),
    CONF_COMMENT("bug fixes, security patches, etc. and not update immediately"),
    CONF_COMMENT("whenever there is a new update available. They will also not"),
    CONF_COMMENT("automatically update when a major _game breaking_ change"),
    CONF_COMMENT("has been introduced."),
    CONF_COMMENT(""),
    CONF_COMMENT("If you want the absolute latest (and EXPERIMENTAL)"),
    CONF_COMMENT("development builds, then use these settings:"),
    CONF_COMMENT("  autoupdate_enabled on"),
    CONF_COMMENT("  autoupdate_branch \"develop\""),
    CONF_COMMENT("  autoupdate_on_major_release yes"),
    CONF_COMMENT("  autoupdate_on_minor_release yes"),
    CONF_COMMENT("  autoupdate_on_patch_release yes"),
    CONF_COMMENT(""),
    CONF_COMMENT("Your server may crash and experience major issues if you"),
    CONF_COMMENT("choose experimental options. You have been warned!"),
    CONF_BLANK(),

    CONF_COMMENT("Automatically update with latest binaries from GitHub?"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: on"),
    CONF_BOOL("autoupdate_enabled", on),
    CONF_BLANK(),

    CONF_COMMENT("Which branch should we pull updates from?"),
    CONF_COMMENT("\"stable\" is recommended for most installations"),
    CONF_COMMENT("\"develop\" may contain experimental features and might cause"),
    CONF_COMMENT("server instability"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    string"),
    CONF_COMMENT("options: \"stable\", \"develop\""),
    CONF_COMMENT("default: \"stable\""),
    CONF_STRING("autoupdate_branch", "stable"),
    CONF_BLANK(),

    CONF_COMMENT("Update on major releases?"),
    CONF_COMMENT("Major releases are generally _game breaking_ updates."),
    CONF_COMMENT("It is usually unsafe to automatically update."),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: no"),
    CONF_BOOL("autoupdate_on_major_release", no),
    CONF_BLANK(),

    CONF_COMMENT("Update on minor releases?"),
    CONF_COMMENT("Minor releases are bug fixes, security patches, and"),
    CONF_COMMENT("new features which DO NOT break compatibility, so they are"),
    CONF_COMMENT("generally safe to update automatically."),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: yes"),
    CONF_BOOL("autoupdate_on_minor_release", yes),
    CONF_BLANK(),

    CONF_COMMENT("Update on patch releases?"),
    CONF_COMMENT("Patch releases are very small incremental updates which might"),
    CONF_COMMENT("contain bug fixes, security patches, etc. If you want"),
    CONF_COMMENT("the absolute _latest_ patches right away, enable this."),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: no"),
    CONF_BOOL("autoupdate_on_patch_release", no),
    CONF_BLANK(),

    CONF_COMMENT("Automatically update when the server has been inactive for"),
    CONF_COMMENT("a while?"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: no"),
    CONF_BOOL("autoupdate_when_inactive", no),
    CONF_BLANK(),

    CONF_COMMENT("If autoupdate_when_inactive is enabled, how long should we"),
    CONF_COMMENT("wait before updating?"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    integer"),
    CONF_COMMENT("options: 60 - 1440 (minutes)"),
    CONF_COMMENT("default: 120 (minutes)"),
    CONF_INT("autoupdate_inactive_delay", 120),
    CONF_BLANK(),

    CONF_END()
};

IC_PUBLIC
struct conf *config_get(void)
{
    return icconf;
}

IC_PUBLIC
int config_log_level(void)
{
    struct confopt *opt = conf_get_opt(icconf, "log_level");

    // something bad has happened
    if (opt == NULL) {
        #ifdef IC_DEBUG
        return LOG_LEVEL_ALL;
        #else
        return LOG_LEVEL_INFO;
        #endif
    }

    // sanity check
    if (opt->value.i > LOG_LEVEL_ALL)
        return LOG_LEVEL_ALL;
    else if (opt->value.i < LOG_LEVEL_NONE)
        return LOG_LEVEL_INFO;
    else
        return opt->value.i;
}

IC_PUBLIC
char *config_console_language(void)
{
    struct confopt *opt = conf_get_opt(icconf, "language");

    if (opt == NULL || opt->value.s == NULL)
        return "english";

    // TEMP until we have multilang support
    return "english";
}

IC_PUBLIC
bool config_log_buffered(void)
{
    struct confopt *opt = conf_get_opt(icconf, "log_buffered");

    if (opt == NULL)
        return true;

    return (opt->value.i == 1);
}

IC_PUBLIC
int config_log_buffer_size(void)
{
    struct confopt *opt = conf_get_opt(icconf, "log_buffer_size");

    if (opt == NULL)
        return 0;

    if (opt->value.i < 0)
        opt->value.i = 0;
    else if (opt->value.i > 1048576)
        opt->value.i = 1048576;

    return opt->value.i;
}

IC_PUBLIC
void config_init(void)
{
    icconf = conf_init("iocod.conf", opts, 0);
    if (icconf == NULL)
        ic_error(_("error creating 'iocod.conf'"));
}

IC_PUBLIC
bool config_initialized(void)
{
    return (icconf != NULL);
}

IC_PUBLIC
void config_shutdown(void)
{
    conf_shutdown(icconf);
}
