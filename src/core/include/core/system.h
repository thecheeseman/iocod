// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include <core/platform.h>
#include <core/types.h>

#include <fmt/format.h>

#include <string>
#include <string_view>

namespace iocod {

struct SystemInfo {
    f32 cpuMhz;
    i32 cpuCores;
    i32 cpuThreads;
    String cpuVendor{};
    String cpuModel{};

    u64 memTotal;
    u64 memFree;
    u64 memAvailable;

    u64 memVirtualSize;
    u64 memVirtualAvailable;
    u64 memVirtualPeak;
    u64 memPhysicalSize;
    u64 memPhysicalPeak;
};

class ISystem {
public:
    virtual ~ISystem() = default;

    /**
     * \brief Initialize the system.
     * \param handle used on win32 to pass the main window handle
     */
    virtual void Initialize(void* handle = nullptr) = 0;
    virtual void Shutdown() noexcept = 0;

    virtual u64 Milliseconds() noexcept = 0;

    virtual String GetConsoleInput() noexcept = 0;
    virtual void ClearConsole() noexcept = 0;
    virtual void FlushConsole() = 0;

    virtual void Print(const String& message) noexcept = 0;
    virtual void DebugPrint(const String& message) noexcept = 0;
    virtual void Error(const String& message) noexcept = 0;
    virtual void Exit(int errorCode) noexcept = 0;

    virtual void PumpEvents() noexcept = 0;

    [[nodiscard]] virtual const SystemInfo& GetSystemInfo() const = 0;
    virtual void PrintSystemInfo() = 0;

    //
    static void LogTrace(const String& message);
    static void LogDebug(const String& message);
    static void LogInfo(const String& message);
    static void LogWarn(const String& message);
    static void LogError(const String& message);
    static void LogErrorNoExit(const String& message);
};

extern ISystem* sys;

template <typename... Args>
inline void LogTrace(const std::string_view& format, Args&&... args)
{
    ISystem::LogTrace(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogDebug(const std::string_view& format, Args&&... args)
{
    ISystem::LogDebug(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogInfo(const std::string_view& format, Args&&... args)
{
    ISystem::LogInfo(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogWarn(const std::string_view& format, Args&&... args)
{
    ISystem::LogWarn(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogError(const std::string_view& format, Args&&... args)
{
    ISystem::LogError(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

template <typename... Args>
inline void LogErrorNoExit(const std::string_view& format, Args&&... args)
{
    ISystem::LogErrorNoExit(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}

} // namespace iocod

#endif // CORE_SYSTEM_H
