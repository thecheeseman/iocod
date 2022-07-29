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

#include "fs_local.h"

IC_NON_NULL(1)
qbool use_localized_searchpath(_In_ searchpath_t *sp)
{
    // always allow non-localized packs
    if (!sp->localized)
        return true;

    // for localized packs:
    // are we ignoring all of them? 
    if (fs_ignore_localized->integer != 0)
        return false;

    // if we're not ignoring them, only match if we're the current language
    if (sp->language == lz_current_language())
        return true;

    return false;
}
