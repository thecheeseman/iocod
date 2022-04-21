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

#ifndef IC_UTILS_H
#define IC_UTILS_H

/* stringify */
#define IC_STRINGIFY_EX(x)      #x
#define IC_STRINGIFY(x)         IC_STRINGIFY_EX(x)

/* concatenation */
#define IC_CONCAT_EX(a, b)      a ## b
#define IC_CONCAT(a, b)         IC_CONCAT_EX(a, b)
#define IC_CONCAT3_EX(a, b, c)  a ## b ## c
#define IC_CONCAT3(a, b, c)     IC_CONCAT3_EX(a, b, c)

/*
 * Some useful utilities from <kernel.h>
 */
#define ARRAY_SIZE(x)       (sizeof(x) / sizeof((x)[0]))
#define FIELD_SIZEOF(t, f)  (sizeof(((t*)0)->f))

#endif /* IC_UTILS_H */
