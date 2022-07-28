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

#ifndef IOCOD_LOCALIZED_H
#define IOCOD_LOCALIZED_H

#include "iocod.h"

IC_PUBLIC
void lz_print_available_languages(void);

IC_PUBLIC
u32 lz_current_language(void);

IC_PUBLIC
IC_NON_NULL(1)
qbool lz_get_language_index(_In_z_ const char *language,
                            _Out_ u32 *language_id);

IC_PUBLIC
IC_RETURNS_STRING
const char *lz_get_language_name(u32 language);

IC_PUBLIC
IC_RETURNS_STRING
IC_NON_NULL(1)
char *lz_get_pak_language(_In_z_ const char *pak_name);

#endif /* IOCOD_LOCALIZED_H */
