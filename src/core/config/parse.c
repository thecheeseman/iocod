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

static bool end_of_script(struct conf *cfg, bool crossline)
{
    if (!crossline) {
        conf_set_error(CONF_ERR_INCOMPLETE, cfg->script_line);
        return false;
    }

    cfg->end = true;
    ic_free(cfg->buffer);
    return false;
}

static bool get_token(struct conf *cfg, bool crossline)
{
    if (cfg->script_p >= cfg->end_p)
        return end_of_script(cfg, crossline);

skipspace:
    while (*cfg->script_p <= ' ') {
        if (cfg->script_p >= cfg->end_p)
            return end_of_script(cfg, crossline);

        if (*cfg->script_p++ == '\n') {
            if (!crossline) {
                conf_set_error(CONF_ERR_INCOMPLETE, cfg->script_line);
                return false;
            }

            cfg->script_line = cfg->line++;
        }
    }

    if (cfg->script_p >= cfg->end_p)
        return end_of_script(cfg, crossline);

    // handle # comments
    if (*cfg->script_p == '#') {
        if (!crossline) {
            conf_set_error(CONF_ERR_INCOMPLETE, cfg->script_line);
            return false;
        }

        while (*cfg->script_p++ != '\n')
            if (cfg->script_p >= cfg->end_p)
                return end_of_script(cfg, crossline);

        cfg->script_line = cfg->line++;
        goto skipspace;
    }

    char *token_p = cfg->token;
    // handle quotes
    if (*cfg->script_p == '"') {
        cfg->script_p++;

        while (*cfg->script_p != '"') {
            *token_p++ = *cfg->script_p++;

            if (cfg->script_p >= cfg->end_p)
                break;

            if (token_p == &cfg->token[MAX_CONFIG_TOKEN]) {
                conf_set_error(CONF_ERR_TOKEN_TOO_LARGE, cfg->script_line);
                return false;
            }
        }

        cfg->script_p++;
    } else {
        while (*cfg->script_p > 32) {
            *token_p++ = *cfg->script_p++;

            if (cfg->script_p >= cfg->end_p)
                break;

            if (token_p == &cfg->token[MAX_CONFIG_TOKEN]) {
                conf_set_error(CONF_ERR_TOKEN_TOO_LARGE, cfg->script_line);
                return false;
            }
        }
    }

    *token_p = 0;

    return true;
}

bool conf_parse(struct conf *cfg)
{
    while (!cfg->end) {
        // out of tokens
        if (!get_token(cfg, true))
            break;

        char optname[MAX_TOKEN_CHARS];
        strncpyz(optname, cfg->token, sizeof(optname));

        bool valid = false;
        struct confopt *opt;
        for (opt = cfg->options; opt->type != CONF_END; opt++) {
            // skip blanks, comments, headers
            if (opt->type == CONF_BLANK || opt->type == CONF_HEADER ||
                opt->type == CONF_SECTION || opt->type == CONF_COMMENT) {
                continue;
            }

            // got it
            if (strcasecmp(opt->name, optname) == 0) {
                valid = true;
                break;
            }
        }

        if (!get_token(cfg, false)) {
            conf_set_error(CONF_ERR_EXPECTED_TOKEN, optname);
            return false; // stop parsing
        }

        if (!valid)
            ic_warning(_("Unknown config option '%s'"), optname);

        // copy data
        switch (opt->type) {
        case CONF_BOOL:
            if (strcasecmp(cfg->token, "yes") == 0 ||
                strcasecmp(cfg->token, "true") == 0 ||
                strcasecmp(cfg->token, "on") == 0) {
                opt->value.i = 1;
            } else if (strcasecmp(cfg->token, "no") == 0 ||
                       strcasecmp(cfg->token, "false") == 0 ||
                       strcasecmp(cfg->token, "off") == 0) {
                opt->value.i = 0;
            } else {
                conf_set_error(CONF_ERR_UNKNOWN_BOOLEAN, cfg->token);
                return false;
            }
            break;
        case CONF_INT:
            opt->value.i = (conf_int) strtol(cfg->token, NULL, 10);
            break;
        case CONF_FLOAT:
            opt->value.f = (conf_float) strtod(cfg->token, NULL);
            break;
        case CONF_STRING:
        case CONF_HEADER:
        case CONF_COMMENT:
            if (opt->value.s != NULL)
                ic_free(opt->value.s);

            opt->value.s = strdup(cfg->token);
            break;
        case CONF_BLANK:
        case CONF_SECTION:
        case CONF_END:
        default:
            break;
        }
    }

    return true;
}
