// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/scoped_timer.h>

#include <cstring>
#include <fstream>
#include <iostream>

namespace iocod {

using Nanoseconds = std::chrono::nanoseconds;
using Seconds = std::chrono::seconds;

namespace {

String ConvertTime(Nanoseconds time)
{
    char fmt[64]{};

    if (time < Nanoseconds(1000))
        snprintf(fmt, sizeof(fmt), "%lld ns", time.count());
    else if (time < Nanoseconds(1000000))
        snprintf(fmt, sizeof(fmt), "%.2g us", static_cast<double>(time.count()) / 1000.0);
    else if (time < Nanoseconds(1000000000))
        snprintf(fmt, sizeof(fmt), "%.2g ms", static_cast<double>(time.count()) / 1000000.0);
    else
        snprintf(fmt, sizeof(fmt), "%.2g s", static_cast<double>(time.count()) / 1000000000.0);

    return fmt;
}

}

void ScopedTimer::DumpToCSV()
{
    static std::size_t counter = 0;
    const String csvname = "scopedtimer" + std::to_string(counter++) + ".csv";
    std::ofstream file(csvname);

    file << "function,calls,total time,min time,max time,avg time,cps,file\n";

    for (const auto& [name, data] : timers) {
        file << data.function << "," << data.function_calls << "," << ConvertTime(data.total_time) << ","
             << ConvertTime(data.min_time) << "," << ConvertTime(data.max_time) << ","
             << ConvertTime(data.total_time / data.function_calls) << ","
             << (Seconds(1) / (data.total_time / data.function_calls)) << "," << data.filename << ":"
             << data.line << "\n";
    }

    file.close();
}

void ScopedTimer::DumpToStdout()
{
    std::cerr << "ScopedTimer data:\n";

    for (const auto& [name, data] : timers) {
        std::cerr << "    " << data.filename << ":" << data.line << " " << data.function << "\n";
        std::cerr << "        calls:      " << data.function_calls << "\n";
        std::cerr << "        total time: " << data.total_time << "\n";
        std::cerr << "        min time:   " << data.min_time << "\n";
        std::cerr << "        max time:   " << data.max_time << "\n";
        std::cerr << "        avg time:   " << data.total_time / data.function_calls << "\n";
    }
}

} // namespace iocod
