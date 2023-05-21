// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include <core/platform.h>
#include <core/system_info.h>
#include <core/types.h>

#include <string>

namespace iocod {

class ISystem {
public:
    virtual ~ISystem() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() noexcept = 0;

    virtual u64 Milliseconds() noexcept = 0;

    virtual std::string GetConsoleInput() noexcept = 0;
    virtual void ClearConsole() noexcept = 0;

    virtual void Print(const std::string& message) noexcept = 0;
    virtual void DebugPrint(const std::string& message) noexcept = 0;
    virtual void Warning(const std::string& message) noexcept = 0;
    virtual void Error(const std::string& message) noexcept = 0;

    virtual SystemInfo GetSystemInfo() = 0;
    virtual void PrintSystemInfo() = 0;

    virtual void AddConsoleCommands() noexcept = 0;
};

extern ISystem* g_system;

} // namespace iocod

#endif // CORE_SYSTEM_H
