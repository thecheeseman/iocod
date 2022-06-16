/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

#ifndef IC_CONFIG_H
#define IC_CONFIG_H

#include "iocod.h"

/**
 * @brief Types of config options.
*/
enum configopt_type {
    CFG_NONE,
    CFG_BLANK,
    CFG_HEADER,
    CFG_COMMENT,
    CFG_BOOL,
    CFG_INT,
    CFG_FLOAT,
    CFG_STRING,
    CFG_END
};

struct configopt {
    const char *name;
    enum configopt_type type;

    char *default_str;
    union value {
        long i;
        double f;
        char *s;
    } value;
};

#define MAX_CONFIG_TOKEN 1024

struct config {
    const char *filename;
    size_t size;

    char *buffer;
    char *script_p;
    char *end_p;

    int line;
    int script_line;

    bool end;

    char token[MAX_CONFIG_TOKEN];

    struct configopt *options;
};

IC_PUBLIC
struct config *config_init(const char *filename, struct configopt *options);

IC_PUBLIC
void config_shutdown(void);

/** @} */

#endif /* IC_CONFIG_H */
