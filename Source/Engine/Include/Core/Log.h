// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_LOG_H
#define CORE_LOG_H

namespace iocod {

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1

#ifdef IOCOD_DEBUG
    #define LOG_DEBUG_ENABLED 1
    #define LOG_TRACE_ENABLED 1
#else
    #define LOG_DEBUG_ENABLED 0
    #define LOG_TRACE_ENABLED 0
#endif

enum class LogLevel {
    Fatal,
    Error,
    Warn,
    Info,
    Debug,
    Trace
};

namespace Log {

IOCOD_API void Initialize();
IOCOD_API void Shutdown();

IOCOD_API void LogMessage(LogLevel level, const char* filename, int line, const char* function,
                          const char* message, ...);

} // namespace Log

} // namespace iocod

#define LogFatal(fmt, ...) \
    iocod::Log::LogMessage(iocod::LogLevel::Fatal, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LogError(fmt, ...) \
    iocod::Log::LogMessage(iocod::LogLevel::Error, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#if LOG_WARN_ENABLED == 1
    #define LogWarn(fmt, ...) \
        iocod::Log::LogMessage(iocod::LogLevel::Warn, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
    #define LogWarn(fmt, ...) ((void) 0)
#endif

#if LOG_INFO_ENABLED == 1
    #define LogInfo(fmt, ...) \
        iocod::Log::LogMessage(iocod::LogLevel::Info, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
    #define LogInfo(fmt, ...) ((void) 0)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define LogDebug(fmt, ...) \
        iocod::Log::LogMessage(iocod::LogLevel::Debug, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
    #define LogDebug(fmt, ...) ((void) 0)
#endif

#if LOG_TRACE_ENABLED == 1
    #define LogTrace(fmt, ...) \
        iocod::Log::LogMessage(iocod::LogLevel::Trace, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
    #define LogTrace(fmt, ...) ((void) 0)
#endif

#endif // CORE_LOG_H
