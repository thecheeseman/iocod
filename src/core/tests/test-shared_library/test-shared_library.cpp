// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/shared_library.h>

#include <catch2/catch_test_macros.hpp>

// ============================================================================
// C++ API
// ============================================================================
TEST_CASE("Can be constructed", "[SharedLibrary]")
{
    iocod::SharedLibrary test;
}

#ifdef _WIN32
TEST_CASE("Loads existing library", "[SharedLibrary]")
{
    iocod::SharedLibrary test;

    REQUIRE(test.Load("kernel32.dll"));
    REQUIRE(test.Loaded());
}

TEST_CASE("Loads existing library on construction", "[SharedLibrary]")
{
    iocod::SharedLibrary test("kernel32.dll");

    REQUIRE(test.Loaded());
}

TEST_CASE("Loads symbol", "[SharedLibrary]")
{
    iocod::SharedLibrary test("kernel32.dll");

    auto GetVersion = test.LoadSymbol<int (*)()>("GetVersion");
    REQUIRE_FALSE(GetVersion == nullptr);
    REQUIRE(GetVersion());
}

TEST_CASE("Fails to load non-existent symbol", "[SharedLibrary]")
{
    iocod::SharedLibrary test("kernel32.dll");

    auto GetVersion = test.LoadSymbol<int (*)()>("GetVersion2");
    REQUIRE(GetVersion == nullptr);
}
#else
TEST_CASE("Loads existing library", "[SharedLibrary]")
{
    iocod::SharedLibrary test;

    REQUIRE(test.Load("libm.so"));
    REQUIRE(test.Loaded());
}

TEST_CASE("Loads existing library on construction", "[SharedLibrary]")
{
    iocod::SharedLibrary test("libm.so");

    REQUIRE(test.Loaded());
}

TEST_CASE("Loads symbol", "[SharedLibrary]")
{
    iocod::SharedLibrary test("libm.so");

    auto pow = test.LoadSymbol<double (*)(double, double)>("pow");
    REQUIRE_FALSE(pow == nullptr);
    REQUIRE(pow(2, 3) == 8);
}

TEST_CASE("Fails to load non-existent symbol", "[SharedLibrary]")
{
    iocod::SharedLibrary math("libm.so");

    const auto sin = math.LoadSymbol<double (*)(double)>("some-non-existent-symbol");
    REQUIRE(sin == nullptr);
}
#endif

TEST_CASE("Fails to load non-existent library", "[SharedLibrary]")
{
    iocod::SharedLibrary test;

    REQUIRE_FALSE(test.Load("some_nonexistant_library"));
    REQUIRE_FALSE(test.Loaded());
}
