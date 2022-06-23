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

static const char illegal_chars[] = { '\\', ';', '\"', '\0'};

/*
 * Check if the given string contains illegal chars.
 */
static bool contains_illegal_chars(const char *str)
{
    for (int i = 0; illegal_chars[i] != '\0'; i++) {
        if (strchr(str, illegal_chars[i]) != NULL) {
            ic_warning(_("Illegal character '%1$c' in key/value '%2$s'\n"),
                       illegal_chars[i], str);
            return true;
        }
    }

    return false;
}

/*
 * Modified version of RTCW code to allow different size keys instead of
 * copy/paste version for infostring/biginfostring
 */
IC_NON_NULL(1, 2, 4, 5)
static bool _if_remove_key(char *s, const char *key, size_t size, 
                           char *pkey, char *value)
{
    if (strlen(s) >= size) {
        ic_error(_("Oversize infostring (>= %lu)\n"), size);
        return false;
    }

    if (contains_illegal_chars(key))
        return false;

    while (true) {
        char *start = s;
        if (*s == '\\')
            s++;

        char *o = pkey;
        while (*s != '\\') {
            if (*s == '\0')
                return false;

            *o++ = *s++;
        }

        *o = '\0';
        s++;

        o = value;

        while (*s != '\\' && *s != '\0') {
            if (*s == '\0')
                return false;

            *o++ = *s++;
        }
        *o = 0;

        if (strcmp(key, pkey) == 0) {
            strcpy(start, s); /* remove this part */
            return true;
        }

        if (*s == '\0')
            break;
    }

    return false;
}

/*
 * Modified version of RTCW code to allow different size keys instead of
 * copy/paste version for infostring/biginfostring
 */
IC_NON_NULL(1, 2, 3, 5)
static bool _if_set_value_for_key(char *s, const char *key, const char *value, 
                                  size_t size, char *newif)
{
    if (strlen(s) >= size) {
        ic_error(_("Oversize infostring (>= %lu)\n"), size);
        return false;
    }

    if (contains_illegal_chars(key) || contains_illegal_chars(value))
        return false;

    if (size <= MAX_INFO_STRING)
        if_remove_key(s, key);
    else
        ifbig_remove_key(s, key);

    if (strlen(value) == 0)
        return false;

    snprintf(newif, size, "\\%s\\%s", key, value);

    if (strlen(newif) + strlen(s) > size) {
        ic_warning(_("Infostring length exceeded (>= %lu)\n"), size);
        return false;
    }

    strncat(s, newif, size);
    return true;
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_remove_key(char *s, const char *key)
{
    char pkey[MAX_INFO_STRING];
    char value[MAX_INFO_STRING];

    return _if_remove_key(s, key, MAX_INFO_STRING, pkey, value);
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_remove_key(char *s, const char *key)
{
    char *pkey = ic_calloc(sizeof(char), INFO_STRING_BIG);
    char *value = ic_calloc(sizeof(char), INFO_STRING_BIG);

    bool ret = _if_remove_key(s, key, INFO_STRING_BIG, pkey, value);

    ic_free(pkey);
    ic_free(value);

    return ret;
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_set_value_for_key(char *s, const char *key, const char *value)
{
    char newif[MAX_INFO_STRING];

    return _if_set_value_for_key(s, key, value, MAX_INFO_STRING, newif);
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_set_value_for_key(char *s, const char *key, const char *value)
{
    char newif[INFO_STRING_BIG];

    return _if_set_value_for_key(s, key, value, INFO_STRING_BIG, newif);
}
