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

#include "com_local.h"

IC_PUBLIC
void ic_print_header(const char *text, size_t size, char sep)
{
    size_t len = strlen(text);
    size_t i;
    size_t j;
    
    if (size < len) {
        log_warn(_("Tried to print header with size smaller than text"));
        return;
    }

    char *header = ic_malloc(size + 1);
    for (i = 0 ; i < size; i++)
        header[i] = sep;
    header[size] = '\0';

    size_t lpad = (size / 2) - (len / 2);
    size_t rpad = (size / 2) + (len / 2);

    if (rpad > size)
        rpad = size;

    if (len > 0) {
        header[lpad - 1] = ' ';

        for (j = 0, i = lpad; i < (len + lpad); i++, j++)
            header[i] = text[j];

        header[i] = ' ';
    }

    ic_printf("%s\n", header);
    ic_free(header);
}
