// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <complex>
#include <limits>

#include <..\Include\Core\NarrowCast.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("narrow_cast", "[narrow_cast]")
{
    int n = 42;
    char c = iocod::NarrowCast<char>(n);
    REQUIRE(c == 42);

    n = 257;
    unsigned char uc = iocod::NarrowCast<unsigned char>(n);
    REQUIRE(uc == 1);
}

TEST_CASE("Narrow w/ exceptions", "[narrow_cast]")
{
    int n = 42;
    char c = iocod::Narrow<char>(n);
    REQUIRE(c == 42);

    n = 257;
    REQUIRE_THROWS_AS(iocod::Narrow<char>(n), iocod::NarrowingError);

    const auto int32_max = std::numeric_limits<int32_t>::max();
    const auto int32_min = std::numeric_limits<int32_t>::min();

    REQUIRE(iocod::Narrow<uint32_t>(int32_t(0)) == 0);
    REQUIRE(iocod::Narrow<uint32_t>(int32_t(1)) == 1);
    REQUIRE(iocod::Narrow<uint32_t>(int32_max) == static_cast<uint32_t>(int32_max));

    REQUIRE_THROWS_AS(iocod::Narrow<uint32_t>(int32_t(-1)), iocod::NarrowingError);
    REQUIRE_THROWS_AS(iocod::Narrow<uint32_t>(int32_min), iocod::NarrowingError);

    n = -42;
    REQUIRE_THROWS_AS(iocod::Narrow<unsigned>(n), iocod::NarrowingError);

    REQUIRE(iocod::Narrow<std::complex<float>>(std::complex<double>(4, 2)) ==
            std::complex<float>(4, 2));
    REQUIRE_THROWS_AS(iocod::Narrow<std::complex<float>>(std::complex<double>(4.2)),
                      iocod::NarrowingError);

    REQUIRE(iocod::Narrow<int>(float(1)) == 1);
}
