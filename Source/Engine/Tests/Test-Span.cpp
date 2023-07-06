// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Containers/Span.h>
#include <doctest/doctest.h>

using namespace iocod;

TEST_CASE("Constructors")
{
    SUBCASE("()")
    {
        constexpr Span<int> span;
        CHECK(span.Size() == 0);
    }

    SUBCASE("(T* ptr, size_t size)")
    {
        constexpr int arr[5]{};
        Span<const int> span(arr, 5);
        CHECK(span.Size() == 5);
        CHECK(span.Data() == arr);
    }

    SUBCASE("(T* begin, T* end)")
    {
        constexpr int arr[5]{};
        Span<const int> span(arr, arr + 5);
        CHECK(span.Size() == 5);
        CHECK(span.Data() == arr);
    }

    SUBCASE("(&)[N]")
    {
        constexpr int arr[5]{};
        Span<const int> span(arr);
        CHECK(span.Size() == 5);
        CHECK(span.Data() == arr);
    }
}

TEST_CASE("operator[]")
{
    constexpr char str[] = "Hello, World!";
    constexpr Span<const char> span(str);

    CHECK(span.Data() == str);
}
