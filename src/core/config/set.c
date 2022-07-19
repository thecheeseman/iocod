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
qbool conf_set_bool(struct conf *cfg, const char *name, qbool value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_BOOL)
        return false;

    opt->value.i = value;
    return true;
}

IC_PUBLIC
qbool conf_set_int(struct conf *cfg, const char *name, conf_int value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_INT)
        return false;

    opt->value.i = value;
    return true;
}

IC_PUBLIC
qbool conf_set_float(struct conf *cfg, const char *name, conf_float value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_FLOAT)
        return false;

    opt->value.f = value;
    return true;
}

IC_PUBLIC
qbool conf_set_string(struct conf *cfg, const char *name, char *value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_STRING)
        return false;

    if (opt->value.s != NULL)
        ic_free(opt->value.s);

    opt->value.s = strdup(value);
    return true;
}
