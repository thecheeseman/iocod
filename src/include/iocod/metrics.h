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

#ifndef IC_METRICS_H
#define IC_METRICS_H

#include "iocod.h"

enum metric_type {
    METRIC_ONCE,
    METRIC_MULTI
};

#ifdef IC_DEBUG
void _metric_begin(enum metric_type type, const char *function,
                   const char *filename, int line);
void _metric_end(const char *function, int line);

#define metric_begin() \
    _metric_begin(METRIC_ONCE, __func__, __FILENAME__, __LINE__)
#define metric_multi_begin() \
    _metric_begin(METRIC_MULTI, __func__, __FILENAME__, __LINE__)
#define metric_end() \
    _metric_end(__func__, __LINE__)
#else
#define metric_begin()
#define metric_multi_begin()
#define metric_end()
#endif

#endif /* IC_METRICS_H */
