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
#include "stringlib.h"

void com_sprintf(char *dest, int size, const char *fmt, ...)
{
    int len;
    va_list argptr;

    va_start(argptr, fmt);
    len = vsnprintf(dest, size, fmt, argptr);
    va_end(argptr);
    dest[size - 1] = '\0';

    if (len >= size)
        com_printf("com_sprintf: overflow of %i in %i ('%s')\n", len, size, 
                   dest);
}

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
    com_sprintf(path, maxsize, "%s%s", oldpath, extension);
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

/*
 * Info strings
 */

char *info_value_for_key(const char *s, const char *key)
{
    char	pkey[BIG_INFO_KEY];
    static	char value[2][BIG_INFO_VALUE];	// use two buffers so compares
                                            // work without stomping on each other
    static	int	valueindex = 0;
    char *o;

    if (!s || !key)
        return "";

    if (strlen(s) >= BIG_INFO_STRING)
        com_error(ERR_DROP, "oversize infostring");

    valueindex ^= 1;
    if (*s == '\\')
        s++;

    while (1) {
        o = pkey;
        while (*s != '\\') {
            if (!*s)
                return "";

            *o++ = *s++;
        }
        *o = 0;
        s++;

        o = value[valueindex];

        while (*s != '\\' && *s)
            *o++ = *s++;

        *o = 0;

        if (strcasecmp(key, pkey) == 0)
            return value[valueindex];

        if (!*s)
            break;

        s++;
    }

    return "";
}

void info_next_pair(const char **head, char *key, char *value) 
{
    char *o;
    const char *s;

    s = *head;

    if (*s == '\\') 
        s++;

    key[0] = 0;
    value[0] = 0;

    o = key;
    while (*s != '\\') {
        if (!*s) {
            *o = 0;
            *head = s;
            return;
        }
        *o++ = *s++;
    }
    *o = 0;
    s++;

    o = value;
    while (*s != '\\' && *s) 
        *o++ = *s++;

    *o = 0;

    *head = s;
}

void info_remove_key(char *s, const char *key) 
{
    char *start;
    char	pkey[MAX_INFO_KEY];
    char	value[MAX_INFO_VALUE];
    char *o;

    if (strlen(s) >= MAX_INFO_STRING)
        com_error(ERR_DROP, "oversize infostring");


    if (strchr(key, '\\'))
        return;

    while (1) {
        start = s;
        if (*s == '\\')
            s++;

        o = pkey;
        while (*s != '\\') {
            if (!*s)
                return;

            *o++ = *s++;
        }
        *o = 0;
        s++;

        o = value;
        while (*s != '\\' && *s) {
            if (!*s)
                return;

            *o++ = *s++;
        }
        *o = 0;

        if (!strcmp(key, pkey)) {
            strcpy(start, s);	// remove this part
            return;
        }

        if (!*s)
            return;
    }
}

void info_remove_key_big(char *s, const char *key) 
{
    char *start;
    char pkey[BIG_INFO_KEY];
    char value[BIG_INFO_VALUE];
    char *o;

    if (strlen(s) >= BIG_INFO_STRING)
        com_error(ERR_DROP, "Info_RemoveKey_Big: oversize infostring");

    if (strchr(key, '\\'))
        return;

    while (1) {
        start = s;
        if (*s == '\\')
            s++;
        o = pkey;
        while (*s != '\\') {
            if (!*s)
                return;
            *o++ = *s++;
        }
        *o = 0;
        s++;

        o = value;
        while (*s != '\\' && *s) {
            if (!*s)
                return;
            *o++ = *s++;
        }
        *o = 0;

        if (!strcmp(key, pkey)) {
            strcpy(start, s);	// remove this part
            return;
        }

        if (!*s)
            return;
    }

}

bool info_validate(const char *s) 
{
    if (strchr(s, '\"'))
        return false;

    if (strchr(s, ';'))
        return false;

    return true;
}

void info_set_value_for_key(char *s, const char *key, const char *value) 
{
    char	newi[MAX_INFO_STRING];

    if (strlen(s) >= MAX_INFO_STRING)
        com_error(ERR_DROP, "Info_SetValueForKey: oversize infostring");

    if (strchr(key, '\\') || strchr(value, '\\')) {
        com_printf("Can't use keys or values with a \\\n");
        return;
    }

    if (strchr(key, ';') || strchr(value, ';')) {
        com_printf("Can't use keys or values with a semicolon\n");
        return;
    }

    if (strchr(key, '\"') || strchr(value, '\"')) {
        com_printf("Can't use keys or values with a \"\n");
        return;
    }

    info_remove_key(s, key);

    if (!value || !strlen(value))
        return;

    com_sprintf(newi, sizeof(newi), "\\%s\\%s", key, value);

    if (strlen(newi) + strlen(s) > MAX_INFO_STRING) {
        com_printf("Info string length exceeded\n");
        return;
    }

    strcat(newi, s);
    strcpy(s, newi);
}

void info_set_value_for_key_big(char *s, const char *key, const char *value) 
{
    char newi[BIG_INFO_STRING];

    if (strlen(s) >= BIG_INFO_STRING)
        com_error(ERR_DROP, "Info_SetValueForKey: oversize infostring");

    if (strchr(key, '\\') || strchr(value, '\\')) {
        com_printf("Can't use keys or values with a \\\n");
        return;
    }

    if (strchr(key, ';') || strchr(value, ';')) {
        com_printf("Can't use keys or values with a semicolon\n");
        return;
    }

    if (strchr(key, '\"') || strchr(value, '\"')) {
        com_printf("Can't use keys or values with a \"\n");
        return;
    }

    info_remove_key_big(s, key);
    if (!value || !strlen(value))
        return;

    com_sprintf(newi, sizeof(newi), "\\%s\\%s", key, value);

    if (strlen(newi) + strlen(s) > BIG_INFO_STRING) {
        com_printf("BIG Info string length exceeded\n");
        return;
    }

    strcat(s, newi);
}

void info_print(const char *s) 
{
    char	key[512];
    char	value[512];
    char *o;
    int		l;

    if (*s == '\\')
        s++;

    while (*s) {
        o = key;
        while (*s && *s != '\\')
            *o++ = *s++;

        l = o - key;
        if (l < 20) {
            memset(o, ' ', 20 - l);
            key[20] = 0;
        } else {
            *o = 0;
        }

        com_printf("%s", key);

        if (!*s) {
            com_printf("MISSING VALUE\n");
            return;
        }

        o = value;
        s++;
        while (*s && *s != '\\')
            *o++ = *s++;
        *o = 0;

        if (*s)
            s++;

        com_printf("%s\n", value);
    }
}
