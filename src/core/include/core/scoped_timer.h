// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SCOPED_TIMER_H
#define CORE_SCOPED_TIMER_H

#include <core/class_non_copyable.h>
#include <core/types.h>

#include <chrono>
#include <map>

namespace iocod {

class ScopedTimer final {
public:
    CLASS_NON_COPYABLE(ScopedTimer)

    explicit ScopedTimer(const char* function_, const char* filename_, u32 line)
        : function(function_),
          filename(filename_),
          line(line),
          start(Clock::now())
    {
        if (!initialized) {
            std::atexit(DumpToCSV);
            initialized = true;
        }
    }

    ~ScopedTimer()
    {
        auto end = Clock::now();
        auto time = std::chrono::duration_cast<Nanoseconds>(end - start);

        auto& data = timers[function];
        data.function = function;
        data.filename = filename;
        data.line = line;
        data.total_time += time;
        data.function_calls++;
        data.min_time = std::min(data.min_time, time);
        data.max_time = std::max(data.max_time, time);
    }

    static void DumpToCSV();
    static void DumpToStdout();

private:
    using Clock = std::chrono::high_resolution_clock;
    using Time = std::chrono::time_point<Clock>;
    using Nanoseconds = std::chrono::nanoseconds;

    Time start;
    const char* filename;
    const char* function;
    u32 line;

    struct TimerData {
        const char* filename;
        const char* function;
        u32 line = 0;
        u32 function_calls = 0;

        Nanoseconds total_time = Nanoseconds(0);
        Nanoseconds min_time = Nanoseconds::max();
        Nanoseconds max_time = Nanoseconds::min();
    };

    static inline std::map<String, TimerData> timers{};
    static inline bool initialized = false;
};

#define CreateScopedTimer() ScopedTimer timer(__FUNCTION__, __FILE__, __LINE__)

} // namespace iocod

#endif // CORE_SCOPED_TIMER_H
