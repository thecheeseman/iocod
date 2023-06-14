// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/scoped_timer.h>

#include <ranges>
#include <fstream>
#include <iostream>

namespace iocod {

using Nanoseconds = std::chrono::nanoseconds;
using Seconds = std::chrono::seconds;

namespace {

String ConvertTime(const Nanoseconds time)
{
    char fmt[64]{};

    if (time < Nanoseconds(1000))
        (void) snprintf(fmt, sizeof(fmt), "%lld ns", time.count());
    else if (time < Nanoseconds(1000000))
        (void) snprintf(fmt, sizeof(fmt), "%.2g us", static_cast<double>(time.count()) / 1000.0);
    else if (time < Nanoseconds(1000000000))
        (void) snprintf(fmt, sizeof(fmt), "%.2g ms", static_cast<double>(time.count()) / 1000000.0);
    else
        (void) snprintf(fmt, sizeof(fmt), "%.2g s", static_cast<double>(time.count()) / 1000000000.0);

    return fmt;
}

}

void ScopedTimer::DumpToCSV()
{
    static std::size_t counter = 0;
    const String csvname = "scopedtimer" + std::to_string(counter++) + ".csv";
    std::ofstream file(csvname);

    file << "m_function,calls,total time,min time,max time,avg time,cps,file\n";

    for (const auto& data : std::views::values(s_timers)) {
        file << data.function << "," << data.functionCalls << "," << ConvertTime(data.totalTime) << ","
             << ConvertTime(data.minTime) << "," << ConvertTime(data.maxTime) << ","
             << ConvertTime(data.totalTime / data.functionCalls) << ","
             << (Seconds(1) / (data.totalTime / data.functionCalls)) << "," << data.filename << ":"
             << data.line << "\n";
    }

    file.close();
}

void ScopedTimer::DumpToStdout()
{
    std::cerr << "ScopedTimer data:\n";

    for (const auto& data : std::views::values(s_timers)) {
        std::cerr << "    " << data.filename << ":" << data.line << " " << data.function << "\n";
        std::cerr << "        calls:      " << data.functionCalls << "\n";
        std::cerr << "        total time: " << data.totalTime << "\n";
        std::cerr << "        min time:   " << data.minTime << "\n";
        std::cerr << "        max time:   " << data.maxTime << "\n";
        std::cerr << "        avg time:   " << data.totalTime / data.functionCalls << "\n";
    }
}

} // namespace iocod
