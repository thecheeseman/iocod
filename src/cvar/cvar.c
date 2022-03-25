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

/**
 * @file cvar.c
 * @date 2022-02-04
*/

#include <string.h>

#include "cvar_local.h"
#include "common/error.h"
#include "common/memory.h"
#include "common/print.h"

struct cvar *cvar_vars;
struct cvar *cvar_cheats;
int cvar_modified_flags;

struct cvar cvar_indexes[MAX_CVARS];
int cvar_num_indexes;

struct cvar *hashtable[FILE_HASH_SIZE];

/**
 * @brief Return a hash value for the given filename
 * @author idsoftware
 * @param fname Name to hash
 * @return A hash of the filename
*/
static int32_t generate_hash_value(const char *fname)
{
    int i;
    int32_t hash;
    char letter;

    if (fname == NULL)
        com_error(ERR_DROP, "null name");

    hash = 0, i = 0;
    while (fname[i] != '\0') {
        letter = tolower(fname[i]);
        hash += (int32_t)(letter) * (i + 119);
        i++;
    }

    hash &= (FILE_HASH_SIZE - 1);
    return hash;
}

/**
 * @brief Validate a cvar string
 * @author idsoftware
 * @param s String to validate
 * @return False if string contains '\' or '"' or ';' chars, true otherwise
*/
static bool cvar_validate_string(const char *s)
{
    if (s == NULL)
        return false;

    if (strchr(s, '\\'))
        return false;

    if (strchr(s, '\"'))
        return false;

    if (strchr(s, ';'))
        return false;

    return true;
}

/**
 * @brief Internal hash table lookup
 * @author idsoftware
*/
struct cvar *cvar_find_var(const char *var_name)
{
    struct cvar *var;
    int32_t hash;

    hash = generate_hash_value(var_name);

    for (var = hashtable[hash]; var != NULL; var = var->hash_next) {
        if (q_stricmp(var_name, var->name) == 0)
            return var;
    }

    return NULL;
}

/**
 * @brief Return a cvar's floating point value
 * @author idsoftware
 * @param var_name Name of the cvar to search for
 * @return A float containing the value
*/
float cvar_variable_value(const char *var_name)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        return 0.0;

    return var->value;
}

/**
 * @brief Return a cvar's integer value
 * @author idsoftware
 * @param var_name Name of the cvar to search for
 * @return An integer containing the value
*/
int cvar_variable_integer_value(const char *var_name)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        return 0;

    return var->integer;
}

/**
 * @brief Return a cvar's string value
 * @author idsoftware
 * @param var_name Name of the cvar to search for
 * @return A string containing the value
*/
char *cvar_variable_string(const char *var_name)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        return "";

    return var->string;
}

/**
 * @brief Return a cvar's string value into the given string buffer
 * @author idsoftware
 * @param var_name Name of the cvar to search for
 * @param buffer Pointer to string buffer 
 * @param bufsize Size of the string buffer
*/
void cvar_variable_string_buffer(const char *var_name, char *buffer, int bufsize)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        *buffer = 0;
    else
        q_strncpyz(buffer, var->string, bufsize);
}

/**
 * @brief 
 * @author idsoftware
 * @param callback 
*/
void cvar_command_completion(void (*callback)(const char *s))
{
    struct cvar *var;

    for (var = cvar_vars; var != NULL; var = var->next)
        callback(var->name);
}

/**
 * @brief Some cvar values need to be safe from foreign characters
 * @author idsoftware
 * @param value Value to clean
 * @return A cleaned string
*/
char *cvar_clean_foreign_characters(const char *value)
{
    static char clean[MAX_CVAR_VALUE_STRING];
    int i, j;

    j = 0;
    for (i = 0; value[i] != '\0'; i++) {
        if (!(value[i] & 128)) {
            clean[j] = value[i];
            j++;
        }
    }
    clean[j] = '\0';

    return clean;
}

/**
 * @brief Return a cvar, or create a new one if it doesn't exist
 * @author idsoftware
*/
struct cvar *cvar_get(const char *var_name, const char *var_value, int flags)
{
    struct cvar *var;
    int32_t hash;

    if (var_name == NULL || var_value == NULL)
        com_error(ERR_FATAL, "cvar_get: NULL parameter");

    if (!cvar_validate_string(var_name)) {
        com_printf("invalid cvar name string: %s\n", var_name);
        var_name = "BADNAME";
    }

    var = cvar_find_var(var_name);
    if (var != NULL) {
        // if the C code is now specifying a variable that the user already
        // set a value for, take the new value as the reset value
        if ((var->flags & CVAR_USER_CREATED) && 
            !(flags & CVAR_USER_CREATED) && 
            var_value[0]) {

            var->flags &= ~CVAR_USER_CREATED;
            z_free(var->reset_string);
            var->reset_string = copy_string(var_value);

            // ZOID--needs to be set so that cvars the game sets as
            // SERVERINFO get sent to clients
            cvar_modified_flags |= flags;
        }

        var->flags |= flags;

        // only allow one non-empty reset string without a warning
        if (*var->reset_string != '\0') {
            // no reset string yet
            z_free(var->reset_string);
            var->reset_string = copy_string(var_value);
        } else if (*var_value != '\0' && 
                   strcmp(var->reset_string, var_value) != 0) {
            com_dprintf("Warning: cvar \"%s\" given initial values: " \
                        "\"%s\" and \"%s\"\n",
                        var_name, var->reset_string, var_value);
        }

        // if we have a latched string, take that value now
        if (var->latched_string != NULL) {
            char *s;

            s = var->latched_string;
            var->latched_string = NULL;
            cvar_set2(var_name, s, true);
            z_free(s);
        }

        if (flags & CVAR_USER_INFO) {
            char *cleaned = cvar_clean_foreign_characters(var->string);
            if (strcmp(var->string, cleaned) != 0)
                cvar_set2(var->name, var->string, false);
        }

        return var;
    }

    if (cvar_num_indexes >= MAX_CVARS)
        com_error(ERR_FATAL, "MAX_CVARS");

    var = &cvar_indexes[cvar_num_indexes];
    cvar_num_indexes++;
    var->name = copy_string(var_name);
    var->string = copy_string(var_value);
    var->modified = true;
    var->modification_count = 1;
    var->value = atof(var->string);
    var->integer = atoi(var->string);
    var->reset_string = copy_string(var_value);

    // link the variable in
    var->next = cvar_vars;
    cvar_vars = var;

    var->flags = flags;

    hash = generate_hash_value(var_name);
    var->hash_next = hashtable[hash];
    hashtable[hash] = var;

    return var;
}

#define FOREIGN_MSG "Foreign charactesr are not allowed in userinfo variables\n"

/**
 * @brief Set a cvar value
*/
struct cvar *cvar_set2(const char *var_name, const char *value, bool force)
{
    struct cvar *var;

    // optional: this could just be com_dprintf but then it woulnd't get logged
    com_printf_logonly("      cvar set % s % s\n", var_name, value);

    if (!cvar_validate_string(var_name))
        com_error(ERR_FATAL, "invalid cvar name string: %s\n", var_name);

    var = cvar_find_var(var_name);
    if (var == NULL) {
        if (value == NULL)
            return NULL;

        // create it
        if (!force)
            return cvar_get(var_name, value, CVAR_USER_CREATED);
        else
            return cvar_get(var_name, value, 0);
    }

    if (value == NULL)
        value = var->reset_string;

    // this was added in RTCW
    // copied here as a safety
    if (var->flags & CVAR_USER_INFO) {
        char *cleaned = cvar_clean_foreign_characters(value);
        if (strcmp(value, cleaned) != 0) {
            com_printf(FOREIGN_MSG);
            com_printf("Using %s instead of %s\n", cleaned, value);
            return cvar_set2(var_name, cleaned, force);
        }
    }

    if (strcmp(value, var->string) == 0)
        return var;

    cvar_modified_flags |= var->flags;

    if (!force) {
        if (var->flags & CVAR_ROM) {
            com_printf("%s is read only.\n", var_name);
            return var;
        }

        if (var->flags & CVAR_INIT) {
            com_printf("%s is write protected.\n", var_name);
            return var;
        }

        if ((var->flags & CVAR_CHEAT) && !cvar_cheats->integer) {
            com_printf("%s is cheated protected.\n", var_name);
            return var;
        }

        if (var->flags & CVAR_LATCH) {
            if (var->latched_string != NULL) {
                if (strcmp(value, var->latched_string) == 0)
                    return var;
                z_free(var->latched_string);
            } else {
                if (strcmp(value, var->string) == 0)
                    return var;
            }

            com_printf("%s will be changed upon restarting.\n", var_name);
            var->latched_string = copy_string(value);
            var->modified = true;
            var->modification_count++;
            return var;
        }
    } else {
        if (var->latched_string != NULL) {
            z_free(var->latched_string);
            var->latched_string = NULL;
        }
    }

    if (!strcmp(value, var->string))
        return var; // not changed

    var->modified = true;
    var->modification_count++;

    z_free(var->string);

    var->string = copy_string(value);
    var->value = atof(var->string);
    var->integer = atoi(var->string);

    return var;
}

void cvar_set(const char *var_name, const char *value)
{
    cvar_set2(var_name, value, true);
}

void cvar_set_latched(const char *var_name, const char *value)
{
    cvar_set2(var_name, value, false);
}

void cvar_set_value(const char *var_name, float value)
{
    char val[32];

    if (value == (int) value)
        com_sprintf(val, sizeof(val), "%i", (int) value);
    else
        com_sprintf(val, sizeof(val), "%f", value);

    cvar_set(var_name, val);
}

void cvar_reset(const char *var_name)
{
    cvar_set2(var_name, NULL, false);
}

void cvar_shutdown(void)
{
    struct cvar *cv;

    for (cv = cvar_vars; cv != NULL; cv = cv->next) {
        if (cv->name != NULL) 
            z_free(cv->name);

        if (cv->string != NULL)
            z_free(cv->string);

        if (cv->latched_string != NULL)
            z_free(cv->latched_string);

        if (cv->reset_string != NULL)
            z_free(cv->reset_string);
    }
}

void cvar_write_defaults(filehandle f)
{
    struct cvar *var;
    char buffer[1024];

    for (var = cvar_vars; var != NULL; var = var->next) {
        // don't write cdkey
        if (q_stricmp(var->name, "cl_cdkey") == 0)
            continue;

        // what
        if (!(var->flags & 0x12c0U)) {
            com_sprintf(buffer, sizeof(buffer), "set %s \"%s\"\n", 
                var->name, var->reset_string);

            fs_printf(f, "%s", buffer);
        }
    }
}

void cvar_write_variables(filehandle f)
{
    struct cvar *var;
    char buffer[1024];

    for (var = cvar_vars; var != NULL; var = var->next) {
        // don't write cdkey
        if (q_stricmp(var->name, "cl_cdkey") == 0)
            continue;

        if (var->flags & CVAR_ARCHIVE) {
            // write the latched value, even if it hasn't taken effect yet
            if (var->latched_string != NULL) {
                com_sprintf(buffer, sizeof(buffer),
                    "seta %s \"%s\"\n", var->name, var->latched_string);
            } else {
                com_sprintf(buffer, sizeof(buffer),
                    "seta %s \"%s\"\n", var->name, var->string);
            }

            fs_printf(f, "%s", buffer);
        }
    }
}

char *cvar_info_string(int bit)
{
    static char info[MAX_INFO_STRING];
    struct cvar *var;

    info[0] = '\0';

    for (var = cvar_vars; var != NULL; var = var->next) {
        if (var->flags & bit)
            info_set_value_for_key(info, var->name, var->string);
    }

    return info;
}

char *cvar_info_string_big(int bit)
{
    static char info[BIG_INFO_STRING];
    struct cvar *var;

    info[0] = '\0';

    for (var = cvar_vars; var != NULL; var = var->next) {
        if (var->flags & bit)
            info_set_value_for_key_big(info, var->name, var->string);
    }

    return info;
}

// only called from sv_setgametype
void cvar_update_flags(void)
{
    struct cvar *var;

    for (var = cvar_vars; var != NULL; var = var->next)
        var->flags &= ~CVAR_8193;
}

void cvar_init(void)
{
    cvar_cheats = cvar_get("sv_cheats", "0", CVAR_ROM | CVAR_SYSTEM_INFO);

    cvar_add_commands();
}
