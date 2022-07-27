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
IC_NON_NULL(1, 2)
conf_t *conf_init(_In_z_ const char *filename,
                  _In_ struct confopt *kv,
                  int options)
{
    UNUSED_PARAM(options);
    #if 0
    if (filename == NULL) {
        conf_set_error(CONF_ERR_NULL_FILENAME);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    if (kv == NULL) {
        conf_set_error(CONF_ERR_NULL_OPTIONS);
        ic_error("%s", conf_error_string());
        return NULL;
    }
    #endif
    struct conf *cfg = ic_calloc(1, sizeof(struct conf));
    cfg->filename = strdup(filename);
    cfg->options = kv;

    if (!conf_set_defaults(cfg) ||
        !conf_write_defaults(cfg) ||
        !conf_load(cfg) ||
        !conf_parse(cfg)) {
        conf_shutdown(cfg);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    return cfg;
}

IC_PUBLIC
qbool conf_shutdown(struct conf *cfg)
{
    if (cfg == NULL)
        return true; // already closed

    ic_free(cfg->filename);

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        if (opt->type == CONF_STRING || opt->type == CONF_HEADER ||
            opt->type == CONF_SECTION || opt->type == CONF_COMMENT) {
            ic_free(opt->value.s);
        }
    }

    ic_free(cfg);

    return true;
}
