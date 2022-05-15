#include "iocod.h"

static const char illegal_chars[] = { '\\', ';', '\"', '\0'};

/**
 * @brief Check if the given string contains illegal chars.
 * @param str[in] string to check
 * @return true if string contains illegal chars
*/
static bool contains_illegal_chars(const char *str)
{
    for (int i = 0; illegal_chars[i] != '\0'; i++) {
        if (strchr(str, illegal_chars[i]) != NULL) {
            ic_warning("illegal character '%c' in key/value '%s'\n",
                       illegal_chars[i], str);
            return true;
        }
    }

    return false;
}

/**
 * Dynamic version of `Info_RemoveKey` from RTCW
 */
IC_NON_NULL(1, 2)
static bool _if_remove_key(char *s, const char *key, size_t size)
{
    if (strlen(s) >= size) {
        ic_error("oversize infostring (>= %lu)\n", size);
        return false;
    }

    if (contains_illegal_chars(key))
        return false;

    char *pkey = ic_calloc(sizeof(char), size);
    if (pkey == NULL) {
        ic_error("error allocating memory\n");
        return false;
    }

    char *value = ic_calloc(sizeof(char), size);
    if (value == NULL) {
        ic_error("error allocating memory\n");
        return false;
    }

    bool ret = false;
    while (true) {
        char *start = s;
        if (*s == '\\')
            s++;

        char *o = pkey;
        while (*s != '\\') {
            if (*s == '\0')
                goto out;

            *o++ = *s++;
        }

        *o = '\0';
        s++;

        o = value;

        while (*s != '\\' && *s != '\0') {
            if (*s == '\0')
                goto out;

            *o++ = *s++;
        }
        *o = 0;

        if (strcmp(key, pkey) == 0) {
            strcpy(start, s); /* remove this part */
            ret = true;
            goto out;
        }

        if (*s == '\0')
            break;
    }

out:
    ic_free(pkey);
    ic_free(value);

    return ret;
}

/**
 * Dynamic version of `Info_SetValueForKey` from RTCW
 */
IC_NON_NULL(1, 2)
static bool _if_set_value_for_key(char *s, const char *key, const char *value, 
                                  size_t size)
{
    if (strlen(s) >= size) {
        ic_error("oversize infostring (>= %lu)\n", size);
        return false;
    }

    if (contains_illegal_chars(key) || contains_illegal_chars(value))
        return false;

    ifbig_remove_key(s, key);
    if (value == NULL || strlen(value) == 0)
        return false;

    char *newif = ic_calloc(sizeof(char), size);
    if (newif == NULL) {
        ic_error("error allocating memory\n");
        return false;
    }
    snprintf(newif, size, "\\%s\\%s", key, value);

    bool ret = false;
    if (strlen(newif) + strlen(s) > size) {
        ic_warning("info string length exceeded (>= %lu)\n", size);
        goto out;
    }

    strncat(s, newif, size);
    ret = true;

out:
    ic_free(newif);
    return ret;
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_remove_key(char *s, const char *key)
{
    return _if_remove_key(s, key, MAX_INFO_STRING);
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_set_value_for_key(char *s, const char *key, const char *value)
{
    return _if_set_value_for_key(s, key, value, MAX_INFO_STRING);
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_remove_key(char *s, const char *key)
{
    return _if_remove_key(s, key, INFO_STRING_BIG);
}

IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_set_value_for_key(char *s, const char *key, const char *value)
{
    return _if_set_value_for_key(s, key, value, INFO_STRING_BIG);
}
