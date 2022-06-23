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
    CONF_HEADER("iocod config"),

    CONF_SECTION("config file syntax notes"),

    CONF_COMMENT("any line that begins with a '#' is treated as a comment"),
    CONF_COMMENT("variables can be booleans, integers, floats, or strings"),
    CONF_COMMENT("  booleans"),
    CONF_COMMENT("    valid values are: true, false, yes, no, on, off"),
    CONF_COMMENT("  integers"),
    CONF_COMMENT("    can be any regular number (42, 25, etc)"),
    CONF_COMMENT("    can also be hexadecimal (0xFE) or octal (031)"),
    CONF_COMMENT("  floats"),
    CONF_COMMENT("    can be any floating number (3.1415, etc)"),
    CONF_COMMENT("    can also use exponents (+1.56E4)"),
    CONF_COMMENT("  strings"),
    CONF_COMMENT("    strings must be enclosed in \"quotes\""),
    CONF_BLANK(),

    CONF_HEADER("general"),

    CONF_COMMENT("set console language"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    string"),
    CONF_COMMENT("options: \"english\""),
    CONF_COMMENT("default: \"english\""),
    CONF_STRING("language", "english"),
    CONF_BLANK(),

    CONF_COMMENT("set log level"),
    CONF_COMMENT("log levels are as listed below:"),
    CONF_COMMENT("  0 = no logging"),
    CONF_COMMENT("  1 = log only fatal errors"),
    CONF_COMMENT("  2 = log ALL errors"),
    CONF_COMMENT("  3 = log ALL errors and warnings"),
    CONF_COMMENT("  4 (default) = log errors, warnings, and useful messages"),
    CONF_COMMENT("  5 = debug messages"),
    CONF_COMMENT("  6 = trace messages (very verbose)"),
    CONF_COMMENT("  7 = log everything"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    integer"),
    CONF_COMMENT("options: 0-7"),
    CONF_COMMENT("default: 4 (errors, warnings + useful messages)"),
    CONF_INT("log_level", 4),
    CONF_BLANK(),

    CONF_HEADER("compatibility"),

    CONF_COMMENT("should we use codextended compatible GSC functions?"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: true"),
    CONF_BOOL("codextended_compat", on),
    CONF_BLANK(),

    CONF_COMMENT("controls which version of cod1 we are targeting"),
    CONF_COMMENT("to target 1.1, set this to 1"),
    CONF_COMMENT("to target 1.5, set this to 5"),
    CONF_COMMENT("to target both, set this to 0"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    integer"),
    CONF_COMMENT("options: 0, 1, 5"),
    CONF_COMMENT("default: 0"),
    CONF_INT("cod_version_target", 0),
    CONF_BLANK(),

    CONF_HEADER("autoupdate"),

    CONF_SECTION("note about autoupdates"),
    CONF_COMMENT("recommended settings for STABLE servers are:"),
    CONF_COMMENT("  autoupdate_enabled on"),
    CONF_COMMENT("  autoupdate_branch \"stable\""),
    CONF_COMMENT("  autoupdate_on_major_release no"),
    CONF_COMMENT("  autoupdate_on_minor_release yes"),
    CONF_COMMENT("  autoupdate_on_patch_release no"),
    CONF_COMMENT(""),
    CONF_COMMENT("the STABLE settings will only update when there are"),
    CONF_COMMENT("bug fixes, security patches, etc. and not update immediately"),
    CONF_COMMENT("whenever there is a new update available. they will also not"),
    CONF_COMMENT("automatically update when a major _game breaking_ change"),
    CONF_COMMENT("has been introduced."),
    CONF_COMMENT(""),
    CONF_COMMENT("if you want the absolute latest (and EXPERIMENTAL)"),
    CONF_COMMENT("development builds, then use these settings:"),
    CONF_COMMENT("  autoupdate_enabled on"),
    CONF_COMMENT("  autoupdate_branch \"develop\""),
    CONF_COMMENT("  autoupdate_on_major_release yes"),
    CONF_COMMENT("  autoupdate_on_minor_release yes"),
    CONF_COMMENT("  autoupdate_on_patch_release yes"),
    CONF_COMMENT(""),
    CONF_COMMENT("your server may crash and experience major issues if you"),
    CONF_COMMENT("choose experimental options. you have been warned!"),
    CONF_BLANK(),

    CONF_COMMENT("automatically update with latest binaries from github?"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: on"),
    CONF_BOOL("autoupdate_enabled", on),
    CONF_BLANK(),

    CONF_COMMENT("which branch should we pull updates from?"),
    CONF_COMMENT("\"stable\" is recommended for most installations"),
    CONF_COMMENT("\"develop\" may contain experimental features and might cause"),
    CONF_COMMENT("server instability"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    string"),
    CONF_COMMENT("options: \"stable\", \"develop\""),
    CONF_COMMENT("default: \"stable\""),
    CONF_STRING("autoupdate_branch", "stable"),
    CONF_BLANK(),

    CONF_COMMENT("update on major releases?"),
    CONF_COMMENT("major releases are generally _game breaking_ updates, so"),
    CONF_COMMENT("it is usually unsafe to automatically update"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: no"),
    CONF_BOOL("autoupdate_on_major_release", no),
    CONF_BLANK(),

    CONF_COMMENT("update on minor releases?"),
    CONF_COMMENT("minor releases are bug fixes, security patches, and"),
    CONF_COMMENT("new features which DO NOT break compatibility, so they are"),
    CONF_COMMENT("generally safe to update automatically"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: yes"),
    CONF_BOOL("autoupdate_on_minor_release", yes),
    CONF_BLANK(),

    CONF_COMMENT("update on patch releases?"),
    CONF_COMMENT("patch releases are very small incremental updates which might"),
    CONF_COMMENT("contain bug fixes, security patches, etc."),
    CONF_COMMENT("if you want the absolute _latest_ patches right away, enable this"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: no"),
    CONF_BOOL("autoupdate_on_patch_release", no),
    CONF_BLANK(),

    CONF_COMMENT("automatically update when the server has been inactive for"),
    CONF_COMMENT("a while?"),
    CONF_COMMENT(""),
    CONF_COMMENT("type:    boolean"),
    CONF_COMMENT("default: no"),
    CONF_BOOL("autoupdate_when_inactive", no),
    CONF_BLANK(),

    CONF_COMMENT("if autoupdate_when_inactive is enabled, how long should we"),
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
    if (opt == NULL)
        return LOG_LEVEL_INFO;

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
