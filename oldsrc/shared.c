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

#include <ctype.h>

#include "shared.h"
#include "common.h"

#include "common/error.h"
#include "common/memory.h"
#include "common/print.h"
#include "strings/stringlib.h"

void com_default_extension(char *path, int maxsize, const char *extension)
{
    char oldpath[MAX_QPATH];
    char *src;

    // if path doesn't have an extension, append
    src = path + strlen(path) - 1;

    while (*src != '/' && src != path) {
        if (*src == '.')
            return; // has an extension

        src--;
    }

    strncpyz(oldpath, path, sizeof(oldpath));
    snprintfz(path, maxsize, "%s%s", oldpath, extension);
}

static short (*_bigshort)(short l);
static short (*_littleshort)(short l);
static int (*_biglong)(int l);
static int (*_littlelong)(int l);
static qint64(*_biglong64)(qint64 l);
static qint64(*_littlelong64)(qint64 l);
static float (*_bigfloat)(float l);
static float (*_littlefloat)(float l);

short   bigshort(short l) { return _bigshort(l); }
short   littleshort(short l) { return _littleshort(l); }
int     biglong(int l) { return _biglong(l); }
int     littlelong(int l) { return _littlelong(l); }
qint64  biglong64(qint64 l) { return _biglong64(l); }
qint64  littlelong64(qint64 l) { return _littlelong64(l); }
float   bigfloat(float l) { return _bigfloat(l); }
float   littlefloat(float l) { return _littlefloat(l); }

short shortswap(short l) 
{
    byte b1, b2;

    b1 = l & 255;
    b2 = (l >> 8) & 255;

    return (b1 << 8) + b2;
}

short shortnoswap(short l) 
{
    return l;
}

int longswap(int l) 
{
    byte b1, b2, b3, b4;

    b1 = l & 255;
    b2 = (l >> 8) & 255;
    b3 = (l >> 16) & 255;
    b4 = (l >> 24) & 255;

    return ((int) b1 << 24) + ((int) b2 << 16) + ((int) b3 << 8) + b4;
}

int longnoswap(int l) 
{
    return l;
}

qint64 long64swap(qint64 ll) 
{
    qint64 result;

    result.b0 = ll.b7;
    result.b1 = ll.b6;
    result.b2 = ll.b5;
    result.b3 = ll.b4;
    result.b4 = ll.b3;
    result.b5 = ll.b2;
    result.b6 = ll.b1;
    result.b7 = ll.b0;

    return result;
}

qint64 long64noswap(qint64 ll) 
{
    return ll;
}

float floatswap(float f) 
{
    union {
        float f;
        byte b[4];
    } dat1, dat2;

    dat1.f = f;
    dat2.b[0] = dat1.b[3];
    dat2.b[1] = dat1.b[2];
    dat2.b[2] = dat1.b[1];
    dat2.b[3] = dat1.b[0];
    return dat2.f;
}

float floatnoswap(float f) 
{
    return f;
}

void swap_init(void)
{
    // 1.1 does not do the swap test but instead just uses arrangement #1
    _bigshort = shortswap;
    _littleshort = shortnoswap;
    _biglong = longswap;
    _littlelong = longnoswap;
    _biglong64 = long64swap;
    _littlelong64 = long64noswap;
    _bigfloat = floatswap;
    _littlefloat = floatnoswap;
}

