// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/String.h>
#include <catch2/catch_test_macros.hpp>

using namespace iocod;

TEST_CASE("String()", "[String]")
{
    const String s;
    REQUIRE(s.Empty());
}

TEST_CASE("String::At access past length", "[String]")
{
    String s;
    REQUIRE_THROWS(s.At(0));
}

TEST_CASE("String::Clear()", "[String]")
{
    String s("hello");
    REQUIRE(s.Length() == 5ull);
    REQUIRE(s == "hello");
    s.Clear();
    REQUIRE(s.Empty());
}

TEST_CASE("String::Fill()", "[String]")
{
    String s(64, '\0');
    REQUIRE(s[0] == '\0');
    s.Fill('a');
    REQUIRE(s[0] == 'a');
    REQUIRE(s[63] == 'a');

    String m(4, '\0');
    m.Fill('x');
    REQUIRE(m[0] == 'x');
}

TEST_CASE("String(size_t, char) < base buffer size", "[String]")
{
    String s(12, 'c');
    REQUIRE(s[0] == 'c');
    REQUIRE(s[11] == 'c');
}

TEST_CASE("String(size_t, char) dynamic allocation", "[String]")
{
    String s(24, 'd');
    REQUIRE(s[0] == 'd');
    REQUIRE(s[23] == 'd');
}

TEST_CASE("String(const char*, size_t) < base buffer size", "[String]")
{
    String s("hello", 5);
    REQUIRE(s == "hello");
    REQUIRE(s.Length() == 5ull);
}

TEST_CASE("String(const char*, size_t) dynamic allocation", "[String]")
{
    String s("this is a much longer string", 28);
    REQUIRE(s == "this is a much longer string");
    REQUIRE(s.Length() == 28ull);
}

TEST_CASE("String(const char*) < base buffer size", "[String]")
{
    String s("hello");
    REQUIRE(s == "hello");
    REQUIRE(s.Length() == 5ull);
}

TEST_CASE("String(const char*) dynamic allocation", "[String]")
{
    String s("this is a much longer string");
    REQUIRE(s == "this is a much longer string");
    REQUIRE(s.Length() == 28ull);
}

TEST_CASE("String(const char*, size_t, size_t) < base buffer size", "[String]")
{
    String s("hello", 1, 2);
    REQUIRE(s == "el");
    REQUIRE(s.Length() == 2ull);
}

TEST_CASE("String(const char*, size_t, size_t) dynamic allocation", "[String]")
{
    String s("this is a much longer string that requires dynamic allocation", 10, 25);
    REQUIRE(s == "much longer string that r");
    REQUIRE(s.Length() == 25ull);
}

TEST_CASE("String() constructors for bool, char, ints, floats", "[String]")
{
    String sbt(true);
    REQUIRE(sbt == "1");

    String sbf(false);
    REQUIRE(sbf == "0");

    String sc1('c');
    REQUIRE(sc1 == "c");

    String sc2('x');
    REQUIRE(sc2 == "x");

    String si321(42);
    REQUIRE(si321 == "42");

    String si312(2147483647);
    REQUIRE(si312 == "2147483647");

    String si641(42ll);
    REQUIRE(si641 == "42");

    String si642(9223372036854775807ll);
    REQUIRE(si642 == "9223372036854775807");

    String sf321(42.0f);
    REQUIRE(sf321 == "42");

    String sf322(69.420f);
    REQUIRE(sf322 == "69.419998");

    String sf641(-123.0);
    REQUIRE(sf641 == "-123");

    String sf642(123.456);
    REQUIRE(sf642 == "123.456");
}

TEST_CASE("String::Append(char)", "[String]")
{
    String s1("hello");
    REQUIRE(s1.Length() == 5ull);
    s1.Append('d');
    REQUIRE(s1.Length() == 6ull);
    REQUIRE(s1[5] == 'd');

    String s2("long string is at least 32 chars");
    REQUIRE(s2.Length() == 32ull);
    s2.Append('!');
    REQUIRE(s2.Length() == 33ull);
    REQUIRE(s2[32] == '!');
}

TEST_CASE("String::Append(const char*)", "[String]")
{
    String s1("small string");
    REQUIRE(s1.Length() == 12ull);
    s1.Append(" is longer");
    REQUIRE(s1.Length() == 22ull);
    REQUIRE(s1 == "small string is longer");

    String s2("slightly longer string that will");
    REQUIRE(s2.Length() == 32ull);
    s2.Append(" require more memory allocations");
    REQUIRE(s2.Length() == 64ull);
    REQUIRE(s2 == "slightly longer string that will require more memory allocations");
}

TEST_CASE("String::operator+=", "[String]")
{
    String s("hello");

    s += " there";
    REQUIRE(s == "hello there");
    s += '!';
    REQUIRE(s == "hello there!");
    s += ' ';

    s += 32;
    REQUIRE(s == "hello there! 32");
    s += ' ';

    s += static_cast<i64>(96);
    REQUIRE(s == "hello there! 32 96");
    s += ' ';

    s += 64u;
    REQUIRE(s == "hello there! 32 96 64");
    s += ' ';

    s += static_cast<u64>(128);
    REQUIRE(s == "hello there! 32 96 64 128");
    s += ' ';

    s += 3.14f;
    REQUIRE(s == "hello there! 32 96 64 128 3.140000");
    s += ' ';

    s += 6.28;
    REQUIRE(s == "hello there! 32 96 64 128 3.140000 6.280000");

    String s2("second string");
    s += s2;
    REQUIRE(s == "hello there! 32 96 64 128 3.140000 6.280000second string");
}

TEST_CASE("String::ToLower & String::ToUpper", "[String]")
{
    String s("tHiS iS a StRiNg");
    s.ToLower();
    REQUIRE(s == "this is a string");
    s.ToUpper();
    REQUIRE(s == "THIS IS A STRING");
}

TEST_CASE("String Wide", "[String]")
{
    String s1("hello");

    wchar_t wide[32]{};
    s1.ToWideString(wide, 32);

    String s2 = String::FromWideString(wide);

    REQUIRE(s1 == s2);

    String s3{s1.ToWideString()};
    REQUIRE(s3 == s1);
}

TEST_CASE("Copy & move", "[String]")
{
    String s1("hello there");
    String s2{s1};
    REQUIRE(s2 == "hello there");

    s2 = "general kenobi";
    s1 = s2;
    REQUIRE(s1 == "general kenobi");

    String s3{std::move(s2)};
    REQUIRE(s3 == "general kenobi");

    String s4 = std::move(s1);
    REQUIRE(s4 == "general kenobi");
}

TEST_CASE("operator/ and /=", "[String]")
{
    String path("test/path");
    path /= "subdir";
    REQUIRE(path == "test/path/subdir");

    String path2 = path / "subdir2";
    REQUIRE(path2 == "test/path/subdir/subdir2");
}
