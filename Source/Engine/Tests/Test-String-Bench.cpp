// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <nanobench.h>
#include <random>
#include <Core/String.h>
#include <catch2/catch_test_macros.hpp>

using namespace iocod;

TEST_CASE("Benchmark")
{
    ankerl::nanobench::Bench bench;
    ankerl::nanobench::Rng rng;

    bench.title("std::string vs String")
         .relative(true)
         .performanceCounters(true);

    auto randomString = [&]() {
        static char s_buffer[32]{};

        for (char c : s_buffer) {
            c = static_cast<char>((rng() % 95) + 32);
        }

        return s_buffer;
    };

    std::string stdstr;

    bench.minEpochIterations(1000000).run("std::string operator=",
                                          [&]() {
                                              ankerl::nanobench::doNotOptimizeAway(stdstr = randomString());
                                          });

    const std::string stdstr2 = randomString();

    bench.minEpochIterations(10000000).run("std::string operator==",
                                           [&]() {
                                               ankerl::nanobench::doNotOptimizeAway(
                                                   stdstr == stdstr2);
                                           });

    String str;

    bench.minEpochIterations(1000000).run("String operator=",
                                          [&]() {
                                              ankerl::nanobench::doNotOptimizeAway(str = randomString());
                                          });

    const String str2 = randomString();

    bench.minEpochIterations(10000000).run("String operator==",
                                           [&]() {
                                               ankerl::nanobench::doNotOptimizeAway(str == str2);
                                           });
}

#if 0
TEST_CASE("benchmark")
{
    ankerl::nanobench::Bench bench;
    ankerl::nanobench::Rng rng;

    bench.title("std::string vs String").relative(true);
    bench.performanceCounters(true);

    String str;
    std::string stdstr;

    bench.minEpochIterations(500).run("std::string operator+=", [&]() {
        stdstr.clear();
        for (const auto size : {8u, 16u, 32u/*, 64u, 128u, 256u, 512u, 1024u*/}) {
            while (stdstr.length() < size) {
                stdstr += std::to_string(rng());
            }
        }
    });

    bench.minEpochIterations(500).run("String operator+=", [&]() {
        str.Clear();
        for (const auto size : {8u, 16u, 32u/*, 64u, 128u, 256u, 512u, 1024u*/}) {
            while (str.Length() < size) {
                str += rng();
            }
        }
    });
}
#endif
