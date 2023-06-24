// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/Log.h>

#include <cstdarg>
#include <cstdio>

namespace iocod::Log {

constexpr const char* LogPrefix(const LogLevel level)
{
    switch (level) {
    case LogLevel::Fatal:
        return "[FATAL] ";
    case LogLevel::Error:
        return "[ERROR] ";
    case LogLevel::Warn:
        return "[WARN]  ";
    case LogLevel::Info:
        return "[INFO]  ";
    case LogLevel::Debug:
        return "[DEBUG] ";
    case LogLevel::Trace:
        return "[TRACE] ";
    }

    IOCOD_UNREACHABLE();
}

void Initialize() { }

void Shutdown() { }

void LogMessage(const LogLevel level, const char* filename, const int line, const char* function,
                const char* message, ...)
{
    bool error = level == LogLevel::Fatal || level == LogLevel::Error;
    static constexpr std::size_t kMessageSize = 4096;

    char msg[kMessageSize]{};
    va_list args;
    va_start(args, message);
    (void) vsnprintf(msg, sizeof(msg) - 10, message, args);
    va_end(args);

    char out[kMessageSize]{};
    (void) snprintf(out, sizeof(out), "%s%s\n", LogPrefix(level), msg);
    (void) fprintf(stderr, "%s", out);
}

} // namespace iocod::Log
