// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/SharedLibrary.h>

#include <catch2/catch_test_macros.hpp>

// ============================================================================
// C++ API
// ============================================================================
TEST_CASE("Can be constructed", "[SharedLibrary]")
{
    SharedLibrary test;
}

#ifdef _WIN32
TEST_CASE("Loads existing library", "[SharedLibrary]")
{
    SharedLibrary test;

    REQUIRE(test.Load("kernel32.dll"));
    REQUIRE(test.Loaded());
}

TEST_CASE("Loads existing library on construction", "[SharedLibrary]")
{
    SharedLibrary test("kernel32.dll");

    REQUIRE(test.Loaded());
}

TEST_CASE("Loads symbol", "[SharedLibrary]")
{
    SharedLibrary test("kernel32.dll");

    auto GetVersion = test.LoadSymbol<int (*)()>("GetVersion");
    REQUIRE_FALSE(GetVersion == nullptr);
    REQUIRE(GetVersion());
}

TEST_CASE("Fails to load non-existent symbol", "[SharedLibrary]")
{
    SharedLibrary test("kernel32.dll");

    auto GetVersion = test.LoadSymbol<int (*)()>("GetVersion2");
    REQUIRE(GetVersion == nullptr);
}
#else
TEST_CASE("Loads existing library", "[SharedLibrary]")
{
    SharedLibrary test;

    REQUIRE(test.Load("libm.so"));
    REQUIRE(test.Loaded());
}

TEST_CASE("Loads existing library on construction", "[SharedLibrary]")
{
    SharedLibrary test("libm.so");

    REQUIRE(test.Loaded());
}

TEST_CASE("Loads symbol", "[SharedLibrary]")
{
    SharedLibrary test("libm.so");

    auto pow = test.LoadSymbol<double (*)(double, double)>("pow");
    REQUIRE_FALSE(pow == nullptr);
    REQUIRE(pow(2, 3) == 8);
}

TEST_CASE("Fails to load non-existent symbol", "[SharedLibrary]")
{
    SharedLibrary math("libm.so");

    const auto sin = math.LoadSymbol<double (*)(double)>("some-non-existent-symbol");
    REQUIRE(sin == nullptr);
}
#endif

TEST_CASE("Fails to load non-existent library", "[SharedLibrary]")
{
    SharedLibrary test;

    REQUIRE_FALSE(test.Load("some_nonexistant_library"));
    REQUIRE_FALSE(test.Loaded());
}

// ============================================================================
// C API
// ============================================================================
TEST_CASE("Can be created", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create(nullptr);

    REQUIRE_FALSE(library == nullptr);

    shared_library_free(library);
}

#ifdef _WIN32
TEST_CASE("Can be created with path", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create("kernel32.dll");

    REQUIRE_FALSE(library == nullptr);
    REQUIRE(shared_library_loaded(library));

    shared_library_free(library);
}

TEST_CASE("Load symbol", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create("kernel32.dll");

    int (*get_version)() =
        reinterpret_cast<int (*)()>(shared_library_load_symbol(library, "GetVersion"));
    REQUIRE_FALSE(get_version == nullptr);
    REQUIRE(get_version());

    shared_library_free(library);
}

TEST_CASE("Fails to load non-existent symbol", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create("kernel32.dll");

    int (*get_version)() =
        reinterpret_cast<int (*)()>(shared_library_load_symbol(library, "GetVersion2"));
    REQUIRE(get_version == nullptr);

    shared_library_free(library);
}
#else
TEST_CASE("Can be created with path", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create("libm.so");

    REQUIRE_FALSE(library == nullptr);
    REQUIRE(shared_library_loaded(library));

    shared_library_free(library);
}

TEST_CASE("Load symbol", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create("libm.so");

    double (*pow)(double, double) =
        reinterpret_cast<double (*)(double, double)>(shared_library_load_symbol(library, "pow"));
    REQUIRE_FALSE(pow == nullptr);
    REQUIRE(pow(2, 3) == 8);

    shared_library_free(library);
}

TEST_CASE("Fails to load non-existent symbol", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create("libm.so");

    void* symbol = shared_library_load_symbol(library, "some-non-existent-symbol");
    REQUIRE(symbol == nullptr);

    shared_library_free(library);
}
#endif

TEST_CASE("Fail to load non-existent library", "[SharedLibrary C API]")
{
    shared_library_t* library = shared_library_create("some_nonexistant_library");

    REQUIRE_FALSE(shared_library_loaded(library));

    shared_library_free(library);
}
