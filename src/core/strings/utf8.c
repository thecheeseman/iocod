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

#include "iocod.h"

IC_PUBLIC
IC_RETURNS_STRING
const char *utf8_decode(_In_z_ const char *text, 
                        _Out_ int *codepoint)
{
    unsigned char c = *text++;

    int extra = 0;
    int min = 0;
    *codepoint = 0;

    if (c >= 0xf0) {
        *codepoint = c & 0x07;
        extra = 3;
        min = 0x10000;
    } else if (c >= 0xe0) {
        *codepoint = c & 0x0f;
        extra = 2;
        min = 0x800;
    } else if (c >= 0xc0) {
        *codepoint = c & 0x1f;
        extra = 1;
        min = 0x80;
    } else {
        *codepoint = c;
    }

    while (extra--) {
        c = *text++;
        if ((c & 0xc0) != 0x80) {
            *codepoint = 0xfffd;
            break;
        }

        (*codepoint) = ((*codepoint) << 6) | (c & 0x3f);
    }

    if (*codepoint < min)
        *codepoint = 0xfffd;

    return text;
}

IC_PUBLIC
IC_RETURNS_STRING
char *utf8_encode(_In_z_ char *text,
                  int codepoint)
{
    if (codepoint < 0 || codepoint > 0x10ffff)
        codepoint = 0xfffd;

    #define EMIT(x, y, z) *text++ = x | ((codepoint >> y) & z)
    if (codepoint < 0x80) { 
        EMIT(0x00, 0, 0x7f); 
    } else if (codepoint < 0x800) { 
        EMIT(0xc0, 6, 0x1f); 
        EMIT(0x80, 0, 0x3f); 
    } else if (codepoint < 0x10000) { 
        EMIT(0xe0, 12, 0x0f); 
        EMIT(0x80, 6, 0x3f); 
        EMIT(0x80, 0, 0x3f); 
    } else { 
        EMIT(0xf0, 18, 0x07); 
        EMIT(0x80, 12, 0x3f); 
        EMIT(0x80, 6, 0x3f); 
        EMIT(0x80, 0, 0x3f); 
    }
    #undef EMIT
    return text;
}

IC_PUBLIC
IC_RETURNS_STRING
const wchar_t *utf16_decode(_In_z_ const wchar_t *text,
                            _Out_ int *codepoint)
{
    int in = *text++;
    if (in < 0xd800 || in > 0xdfff) 
        *codepoint = in;
    else if (in > 0xD800 && in < 0xDBFF) 
        *codepoint = ((in & 0x03ff) << 10) | (*text++ & 0x03ff);
    else *codepoint = 0xfffd;

    return text;
}

IC_PUBLIC
IC_RETURNS_STRING
wchar_t *utf16_encode(_In_z_ wchar_t *text, 
                      int codepoint)
{
    if (codepoint < 0x10000) {
        *text++ = codepoint;
    }  else {
        codepoint -= 0x10000;
        *text++ = 0xd800 | ((codepoint >> 10) & 0x03ff);
        *text++ = 0xdc00 | (codepoint & 0x03ff);
    }

    return text;
}

IC_PUBLIC
void utf8_widen(_In_z_ const char *in, 
                _Out_ wchar_t *out)
{
    int codepoint;

    while (*in) {
        in = utf8_decode(in, &codepoint);
        out = utf16_encode(out, codepoint);
    }
}

IC_PUBLIC
void utf16_shorten(_In_z_ const wchar_t *in, 
                   _Out_ char *out)
{
    int codepoint;
    
    while (*in) {
        in = utf16_decode(in, &codepoint);
        out = utf8_encode(out, codepoint);
    }
}
