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

#include "log_local.h"

IC_PUBLIC
void log_set_buffered(bool buffered, int buffer_size)
{
    iclog.buffered = buffered;
    iclog.buffer_size = buffer_size;
    
    if (iclog.buffer_size < 0)
        iclog.buffer_size = 0;

    if (iclog.buffer != NULL)
        ic_free(iclog.buffer);

    if (buffered) {
        if (iclog.buffer_size > 0) {
            // custom buffer
            iclog.buffer = ic_malloc(iclog.buffer_size);
            setvbuf(iclog.fp, iclog.buffer, _IOFBF, iclog.buffer_size);
        } else {
            // let OS determine buffer
            setvbuf(iclog.fp, NULL, _IOFBF, BUFSIZ);
        }
    } else {
        // unbuffered
        setvbuf(iclog.fp, NULL, _IONBF, 0);
    }
}
