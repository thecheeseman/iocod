// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include <core/platform.h>
#include <core/system_info.h>
#include <core/types.h>

#include <fmt/format.h>

#include <string>

namespace iocod {

class ISystem {
public:
    virtual ~ISystem() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() noexcept = 0;

    virtual u64 Milliseconds() noexcept = 0;

    virtual String GetConsoleInput() noexcept = 0;
    virtual void ClearConsole() noexcept = 0;

    virtual void Print(const String& message) noexcept = 0;
    virtual void DebugPrint(const String& message) noexcept = 0;
    virtual void Warning(const String& message) noexcept = 0;
    virtual void Error(const String& message) noexcept = 0;

    virtual SystemInfo GetSystemInfo() = 0;
    virtual void PrintSystemInfo() = 0;

    //
    static void LogTrace(const String& message);
    static void LogDebug(const String& message);
    static void LogInfo(const String& message);
    static void LogWarn(const String& message);
    static void LogError(const String& message);
    static void LogErrorNoExit(const String& message);
};

extern ISystem* System;

template <typename... Args>
inline void LogTrace(const String& format, Args&&... args)
{
    ISystem::LogTrace(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogDebug(const String& format, Args&&... args)
{
    ISystem::LogDebug(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogInfo(const String& format, Args&&... args)
{
    ISystem::LogInfo(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogWarn(const String& format, Args&&... args)
{
    ISystem::LogWarn(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogError(const String& format, Args&&... args)
{
    ISystem::LogError(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogErrorNoExit(const String& format, Args&&... args)
{
    ISystem::LogErrorNoExit(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

} // namespace iocod

#endif // CORE_SYSTEM_H
