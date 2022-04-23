#include "iocod.h"

#ifndef INFOSTRING_STANDALONE
#include "common/error.h"
#include "common/print.h"
#endif

#include "strings/stringlib.h"
#include "types/bool.h"
#include "infostring.h"

char *info_value_for_key(const char *s, const char *key)
{
    char pkey[BIG_INFO_KEY];
    static char value[2][BIG_INFO_VALUE];	// use two buffers so compares
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
    char pkey[MAX_INFO_KEY];
    char value[MAX_INFO_VALUE];
    char *o;

    if (strlen(s) >= MAX_INFO_STRING)
        com_error(ERR_DROP, "oversize infostring");


    if (strchr(key, '\\'))
        return;

    while (1) {
        char *start = s;
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
    char pkey[BIG_INFO_KEY];
    char value[BIG_INFO_VALUE];
    char *o;

    if (strlen(s) >= BIG_INFO_STRING)
        com_error(ERR_DROP, "Info_RemoveKey_Big: oversize infostring");

    if (strchr(key, '\\'))
        return;

    while (1) {
        char *start = s;
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
    char newi[MAX_INFO_STRING];

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

    snprintfz(newi, sizeof(newi), "\\%s\\%s", key, value);

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

    snprintfz(newi, sizeof(newi), "\\%s\\%s", key, value);

    if (strlen(newi) + strlen(s) > BIG_INFO_STRING) {
        com_printf("BIG Info string length exceeded\n");
        return;
    }

    strcat(s, newi);
}

void info_print(const char *s)
{
    char key[512];
    char value[512];

    if (*s == '\\')
        s++;

    while (*s) {
        char *o = key;
        while (*s && *s != '\\')
            *o++ = *s++;

        int l = o - key;
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
