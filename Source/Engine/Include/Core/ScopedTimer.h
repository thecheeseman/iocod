// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SCOPED_TIMER_H
#define CORE_SCOPED_TIMER_H

#include <chrono>
#include <map>

namespace iocod {

class ScopedTimer final {
public:
    CLASS_NON_COPYABLE(ScopedTimer)

    explicit ScopedTimer(const char* function, const char* filename, const u32 line)
        : m_start(Clock::now()),
          m_filename(filename),
          m_function(function),
          m_line(line)
    {
        if (!s_initialized) {
            (void) std::atexit(DumpToCSV);
            s_initialized = true;
        }
    }

    ~ScopedTimer()
    {
        const auto end = Clock::now();
        const auto time = std::chrono::duration_cast<Nanoseconds>(end - m_start);

        auto& data = s_timers[m_function];
        data.function = m_function;
        data.filename = m_filename;
        data.line = m_line;
        data.totalTime += time;
        data.functionCalls++;
        data.minTime = std::min(data.minTime, time);
        data.maxTime = std::max(data.maxTime, time);
    }

    static void DumpToCSV();
    static void DumpToStdout();

private:
    using Clock = std::chrono::high_resolution_clock;
    using Time = std::chrono::time_point<Clock>;
    using Nanoseconds = std::chrono::nanoseconds;

    Time m_start;
    const char* m_filename;
    const char* m_function;
    u32 m_line;

    struct TimerData {
        const char* filename{nullptr};
        const char* function{nullptr};
        u32 line{0};
        u32 functionCalls{0};

        Nanoseconds totalTime = Nanoseconds(0);
        Nanoseconds minTime = Nanoseconds::max();
        Nanoseconds maxTime = Nanoseconds::min();
    };

    static inline std::map<const char*, TimerData> s_timers{};
    static inline bool s_initialized = false;
};

#define CreateScopedTimer() ScopedTimer timer(__FUNCTION__, __FILE__, __LINE__)

} // namespace iocod

#endif // CORE_SCOPED_TIMER_H
