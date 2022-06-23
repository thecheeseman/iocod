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

IC_PUBLIC
struct confopt *conf_get_opt(struct conf *cfg, const char *name)
{
    if (cfg == NULL || name == NULL)
        return NULL;

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        if (strcasecmp(opt->name, name) == 0)
            return opt;
    }

    return NULL;
}

IC_PUBLIC
bool conf_get_bool(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_BOOL)
        return false;

    return (opt->value.i != 0);
}

IC_PUBLIC
conf_int conf_get_int(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_INT)
        return 0;

    return opt->value.i;
}

IC_PUBLIC
conf_float conf_get_float(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_FLOAT)
        return 0.0;

    return opt->value.f;
}

IC_PUBLIC
char *conf_get_string(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_STRING)
        return NULL;

    return opt->value.s;
}
