// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <complex>
#include <limits>

#include <core/narrow_cast.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("narrow_cast", "[narrow_cast]")
{
    int n = 42;
    char c = iocod::narrow_cast<char>(n);
    REQUIRE(c == 42);

    n = 257;
    unsigned char uc = iocod::narrow_cast<unsigned char>(n);
    REQUIRE(uc == 1);
}

TEST_CASE("Narrow w/ exceptions", "[narrow_cast]")
{
    int n = 42;
    char c = iocod::narrow<char>(n);
    REQUIRE(c == 42);

    n = 257;
    REQUIRE_THROWS_AS(iocod::narrow<char>(n), iocod::narrowing_error);

    const auto int32_max = std::numeric_limits<int32_t>::max();
    const auto int32_min = std::numeric_limits<int32_t>::min();

    REQUIRE(iocod::narrow<uint32_t>(int32_t(0)) == 0);
    REQUIRE(iocod::narrow<uint32_t>(int32_t(1)) == 1);
    REQUIRE(iocod::narrow<uint32_t>(int32_max) == static_cast<uint32_t>(int32_max));

    REQUIRE_THROWS_AS(iocod::narrow<uint32_t>(int32_t(-1)), iocod::narrowing_error);
    REQUIRE_THROWS_AS(iocod::narrow<uint32_t>(int32_min), iocod::narrowing_error);

    n = -42;
    REQUIRE_THROWS_AS(iocod::narrow<unsigned>(n), iocod::narrowing_error);

    REQUIRE(iocod::narrow<std::complex<float>>(std::complex<double>(4, 2)) ==
            std::complex<float>(4, 2));
    REQUIRE_THROWS_AS(iocod::narrow<std::complex<float>>(std::complex<double>(4.2)),
                      iocod::narrowing_error);

    REQUIRE(iocod::narrow<int>(float(1)) == 1);
}
