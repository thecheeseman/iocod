// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch_test_macros.hpp>
#include <..\Include\Core\Timer.h>
#include <thread>

using namespace iocod;

TEST_CASE("construction", "timer")
{
    Timer t;
}

TEST_CASE("run for 1ms", "timer")
{
    Timer t;

    t.Start();
    std::this_thread::sleep_for(Milliseconds(1));
    t.Stop();

    REQUIRE(t.Milliseconds() >= 1u);
    REQUIRE(t.Microseconds() >= 1000u);
    REQUIRE(t.Nanoseconds() >= 1000000u);
}
