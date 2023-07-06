// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_EASTL_IOCOD_H
#define ENGINE_EASTL_IOCOD_H

#include <EABase/eabase.h>

#define EASTL_API IOCOD_API
#define EASTL_LOCAL IOCOD_LOCAL

#ifdef IOCOD_DEBUG
#define EASTL_DEBUG 1
#endif

#define EASTL_USER_DEFINED_ALLOCATOR 1
#define EASTL_ASSERT Assert

namespace eastl {

class allocator;

inline bool operator==(const allocator&, const allocator&) { return true; }
inline bool operator!=(const allocator&, const allocator&) { return false; }

} // namespace eastl

#endif // ENGINE_EASTL_IOCOD_H
