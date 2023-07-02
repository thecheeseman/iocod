// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/String.h>
#include <doctest/doctest.h>

using namespace iocod;

TEST_CASE("String()")
{
    const String s;

    REQUIRE(s.Empty());
}

TEST_CASE("String::At access past length")
{
    String s;

    REQUIRE_THROWS(s.At(0));
}

TEST_CASE("String::Clear()")
{
    String s("hello");

    CHECK(s.Length() == 5ull);
    CHECK(s == "hello");

    s.Clear();
    REQUIRE(s.Empty());
}

TEST_CASE("String::Fill()")
{
    String s(64, '\0');
    REQUIRE(s[0] == '\0');

    s.Fill('a');

    constexpr auto expected = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    REQUIRE(s == expected);
}

TEST_CASE("String(size_t, char) < base buffer size")
{
    const String s(12, 'c');

    constexpr auto expected = "cccccccccccc";
    REQUIRE(s == expected);
}

TEST_CASE("String(size_t, char) dynamic allocation")
{
    const String s(48, 'd');

    constexpr auto expected = "dddddddddddddddddddddddddddddddddddddddddddddddd";
    REQUIRE(s == expected);

}

TEST_CASE("String(const char*, size_t) < base buffer size")
{
    String s("hello", 5);

    CHECK(s.Length() == 5ull);
    REQUIRE(s == "hello");
}

TEST_CASE("String(const char*, size_t) dynamic allocation")
{
    String s("this is a much longer string", 28);

    CHECK(s.Length() == 28ull);
    REQUIRE(s == "this is a much longer string");
}

TEST_CASE("String(const char*) < base buffer size")
{
    String s("hello");

    CHECK(s.Length() == 5ull);
    REQUIRE(s == "hello");
}

TEST_CASE("String(const char*) dynamic allocation")
{
    String s("this is a much longer string");

    CHECK(s.Length() == 28ull);
    REQUIRE(s == "this is a much longer string");
}

TEST_CASE("String(const char*, size_t, size_t) < base buffer size")
{
    String s("hello", 1, 2);

    CHECK(s.Length() == 2ull);
    REQUIRE(s == "el");
}

TEST_CASE("String(const char*, size_t, size_t) dynamic allocation")
{
    String s("this is a much longer string that requires dynamic allocation", 10, 25);

    CHECK(s.Length() == 25ull);
    REQUIRE(s == "much longer string that r");
}

TEST_CASE("String() constructors for bool, char, ints, floats")
{
    SUBCASE("String(bool)")
    {
        const String sbt(true);
        CHECK(sbt.Length() == 1);
        REQUIRE(sbt == "1");

        const String sbf(false);
        CHECK(sbf.Length() == 1);
        REQUIRE(sbf == "0");
    }

    SUBCASE("String(char)")
    {
        const String sc1('c');
        CHECK(sc1.Length() == 1);
        REQUIRE(sc1 == "c");

        const String sc2('x');
        CHECK(sc2.Length() == 1);
        REQUIRE(sc2 == "x");
    }

    SUBCASE("String(i32)")
    {
        const String si321(42);
        CHECK(si321.Length() == 2);
        REQUIRE(si321 == "42");

        const String si312(2147483647);
        CHECK(si312.Length() == 10);
        REQUIRE(si312 == "2147483647");
    }

    SUBCASE("String(i64)")
    {
        const String si641(42ll);
        CHECK(si641.Length() == 2);
        REQUIRE(si641 == "42");

        const String si642(9223372036854775807ll);
        CHECK(si642.Length() == 19);
        REQUIRE(si642 == "9223372036854775807");
    }

    SUBCASE("String(u32)")
    {
        const String si321(42u);
        CHECK(si321.Length() == 2);
        REQUIRE(si321 == "42");

        const String si312(2147483647u);
        CHECK(si312.Length() == 10);
        REQUIRE(si312 == "2147483647");
    }

    SUBCASE("String(u64)")
    {
        const String si641(42ull);
        CHECK(si641.Length() == 2);
        REQUIRE(si641 == "42");

        const String si642(18446744073709551615ull);
        CHECK(si642.Length() == 20);
        REQUIRE(si642 == "18446744073709551615");
    }

    SUBCASE("String(f32)")
    {
        const String sf321(42.0f);
        CHECK(sf321.Length() == 2);
        REQUIRE(sf321 == "42");

        const String sf322(69.420f);
        CHECK(sf322.Length() == 9);
        REQUIRE(sf322 == "69.419998");
    }

    SUBCASE("String(f64)")
    {
        const String sf641(-123.0);
        CHECK(sf641.Length() == 4);
        REQUIRE(sf641 == "-123");

        const String sf642(123.456);
        CHECK(sf642.Length() == 7);
        REQUIRE(sf642 == "123.456");
    }
}

TEST_CASE("String::Append(char)")
{
    SUBCASE("Append < base buffer size")
    {
        String s1("hello");

        CHECK(s1.Length() == 5ull);

        s1.Append('d');
        REQUIRE(s1.Length() == 6ull);
        REQUIRE(s1[5] == 'd');
    }

    SUBCASE("Append > base buffer size")
    {
        String s2("long string is at the very least 40 chars");

        CHECK(s2.Length() == 41ull);

        s2.Append('!');
        REQUIRE(s2.Length() == 42ull);
        REQUIRE(s2[41] == '!');
    }
}

TEST_CASE("String::Append(const char*)")
{
    SUBCASE("Append < base buffer size")
    {
        String s1("small string");
        CHECK(s1.Length() == 12ull);

        s1.Append(" is longer");
        REQUIRE(s1.Length() == 22ull);
        REQUIRE(s1 == "small string is longer");
    }

    SUBCASE("Append > base buffer size")
    {
        String s2("slightly longer string that will assuredly");
        CHECK(s2.Length() == 42ull);

        s2.Append(" require more memory allocations");
        REQUIRE(s2.Length() == 74ull);
        REQUIRE(s2 == "slightly longer string that will assuredly require more memory allocations");
    }
}

TEST_CASE("String::operator+=")
{
    String s("hello");

    SUBCASE("char")
    {
        s += " there";
        REQUIRE(s == "hello there");
        s += '!';
        REQUIRE(s == "hello there!");
        s += ' ';
    }

    SUBCASE("i32")
    {
        s += 32;
        REQUIRE(s == "hello32");
        s += ' ';
    }

    SUBCASE("i64")
    {
        s += static_cast<i64>(96);
        REQUIRE(s == "hello96");
        s += ' ';
    }

    SUBCASE("u32")
    {
        s += 64u;
        REQUIRE(s == "hello64");
        s += ' ';
    }

    SUBCASE("u64")
    {
        s += static_cast<u64>(128);
        REQUIRE(s == "hello128");
        s += ' ';
    }

    SUBCASE("f32")
    {
        s += 3.14f;
        REQUIRE(s == "hello3.140000");
        s += ' ';
    }

    SUBCASE("f64")
    {
        s += 6.28;
        REQUIRE(s == "hello6.280000");
    }

    SUBCASE("string")
    {
        const String s2("second string");
        s += s2;
        REQUIRE(s == "hellosecond string");
    }
}

TEST_CASE("String::ToLower & String::ToUpper")
{
    String s("tHiS iS a StRiNg");

    s.ToLower();
    REQUIRE(s == "this is a string");
    s.ToUpper();
    REQUIRE(s == "THIS IS A STRING");
}

TEST_CASE("String Wide")
{
    String s1("hello");

    wchar_t wide[32]{};
    s1.ToWideString(wide, 32);

    String s2 = String::FromWideString(wide);

    REQUIRE(s1 == s2);

    String s3{s1.ToWideString()};
    REQUIRE(s3 == s1);
}

TEST_CASE("Copy & move")
{
    String s1("hello there");
    String s2{s1};
    CHECK(s2 == "hello there");

    s2 = "general kenobi";
    s1 = s2;
    REQUIRE(s1 == "general kenobi");

    String s3{std::move(s2)};
    REQUIRE(s3 == "general kenobi");

    String s4 = std::move(s1);
    REQUIRE(s4 == "general kenobi");
}

TEST_CASE("operator/ and /=")
{
    String path("test/path");
    path /= "subdir";
    REQUIRE(path == "test/path/subdir");

    String path2 = path / "subdir2";
    REQUIRE(path2 == "test/path/subdir/subdir2");
}
