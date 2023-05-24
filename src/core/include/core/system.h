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

    virtual std::string GetConsoleInput() noexcept = 0;
    virtual void ClearConsole() noexcept = 0;

    virtual void Print(const std::string& message) noexcept = 0;
    virtual void DebugPrint(const std::string& message) noexcept = 0;
    virtual void Warning(const std::string& message) noexcept = 0;
    virtual void Error(const std::string& message) noexcept = 0;

    virtual SystemInfo GetSystemInfo() = 0;
    virtual void PrintSystemInfo() = 0;

    //
    static void LogTrace(const std::string& message);
    static void LogDebug(const std::string& message);
    static void LogInfo(const std::string& message);
    static void LogWarn(const std::string& message);
    static void LogError(const std::string& message);
};

extern ISystem* g_system;

template <typename... Args>
inline void LogTrace(const std::string& format, Args&&... args)
{
    ISystem::LogTrace(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogDebug(const std::string& format, Args&&... args)
{
    ISystem::LogDebug(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogInfo(const std::string& format, Args&&... args)
{
    ISystem::LogInfo(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogWarn(const std::string& format, Args&&... args)
{
    ISystem::LogWarn(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogError(const std::string& format, Args&&... args)
{
    ISystem::LogError(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

} // namespace iocod

#endif // CORE_SYSTEM_H
