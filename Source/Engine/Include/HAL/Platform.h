// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_HAL_PLATFORM_H
#define ENGINE_HAL_PLATFORM_H

#include <Core/String.h>

namespace iocod {

namespace Platform {

IOCOD_API void Initialize();
IOCOD_API void Shutdown();
IOCOD_API String GetLastErrorAsString();
IOCOD_API [[noreturn]] void DisplayFatalErrorAndExit();

} // namespace Platform

} // namespace iocod

#endif //ENGINE_HAL_PLATFORM_H
