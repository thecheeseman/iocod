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

static const char *opttypestr[] = {
    "",
    "comment",
    "header",
    "section",
    "boolean",
    "integer",
    "float",
    "string",
    ""
};

IC_PUBLIC
void conf_dump_options(struct conf *cfg)
{
    printf("%-16s %-32s %s\n", "type", "name", "value");

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        char value[256];
        bool print_name = true;

        switch (opt->type) {
        case CONF_HEADER:
        case CONF_SECTION:
        case CONF_COMMENT:
            print_name = false;

            if (opt->type == CONF_COMMENT)
                snprintf(value, sizeof(value), "# %s", opt->value.s);
            else
                snprintf(value, sizeof(value), "%s", opt->value.s);
            break;
        case CONF_BOOL:
            if (opt->value.i)
                snprintf(value, sizeof(value), "true");
            else
                snprintf(value, sizeof(value), "false");
            break;
        case CONF_INT:
            snprintf(value, sizeof(value), "%d", opt->value.i);
            break;
        case CONF_FLOAT:
            snprintf(value, sizeof(value), "%g", opt->value.f);
            break;
        case CONF_STRING:
            snprintf(value, sizeof(value), "\"%s\"", opt->value.s);
            break;
        case CONF_BLANK:
        case CONF_END:
        default:
            break;
        }

        if (opt->type == CONF_BLANK) {
            printf("\n");
        } else {
            printf("%-16s %-32s %s\n",
                   opt->type <= CONF_END ? opttypestr[opt->type] : "bad type",
                   print_name ? opt->name : "",
                   value);
        }
    }
}
