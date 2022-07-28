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

static const char *localized_languages[] = {
    "english",
    "french",
    "german",
    "italian",
    "spanish",
    "british",
    "russian",
    "polish",
    "korean",
    "taiwanese",
    "japanese",
    "chinese",
    "thai",
    "leet"
};
static const u32 localized_language_count = ARRAY_SIZE(localized_languages);

IC_PUBLIC
void lz_print_available_languages(void)
{
    ic_printf(_("Supported languages are:\n"));
    for (u32 i = 0; i < localized_language_count; i++) {
        ic_printf("    %s\n", localized_languages[i]);
    }
}

IC_PUBLIC
u32 lz_current_language(void)
{
    return cl_language->integer;
}

IC_PUBLIC
IC_NON_NULL(1)
qbool lz_get_language_index(_In_z_ const char *language,
                            _Out_ u32 *language_id)
{
    for (u32 i = 0; i < localized_language_count; i++) {
        if (strcasecmp(language, localized_languages[i]) == 0) {
            *language_id = i;
            return true;
        }
    }
    
    *language_id = 0;
    return false;
}

IC_PUBLIC
IC_RETURNS_STRING
const char *lz_get_language_name(u32 language)
{
    if (language >= 0 && language < localized_language_count)
        return localized_languages[language];

    return "english";
}

IC_PUBLIC
IC_RETURNS_STRING
IC_NON_NULL(1)
char *lz_get_pak_language(_In_z_ const char *pak_name)
{
    static int id = 0;
    static char paks[2][MAX_OSPATH];

    id ^= 1;

    if (strlen(pak_name) < 10) {
        paks[id][0] = '\0';
    } else {
        memset(paks[id], 0, MAX_OSPATH);

        int i = 10;
        while (pak_name[i] != '\0') {
            if (i >= MAX_OSPATH)
                break;

            if (pak_name[i] == '_')
                break;

            paks[id][i - 10] = pak_name[i];
            i++;
        }
    }

    return paks[id];
}
