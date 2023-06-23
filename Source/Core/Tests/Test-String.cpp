// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/String.h>
#include <catch2/catch_test_macros.hpp>

using namespace iocod;

TEST_CASE("String()", "[String]")
{
    const FString s;

    REQUIRE(s.Empty());
}

TEST_CASE("String::At access past length", "[String]")
{
    FString s;

    REQUIRE_THROWS(s.At(0));
}

TEST_CASE("String::Clear()", "[String]")
{
    FString s("hello");

    REQUIRE(s.Length() == 5);
    REQUIRE(s[0] == 'h');
    REQUIRE(s[4] == 'o');
    s.Clear();
    REQUIRE(s.Empty());
}

TEST_CASE("String::Fill()", "[String]")
{
    FString s(64, '\0');
    REQUIRE(s[0] == '\0');
    s.Fill('a');
    REQUIRE(s[0] == 'a');
    REQUIRE(s[63] == 'a');

    FString m(4, '\0');
    m.Fill('x');
    REQUIRE(m[0] == 'x');
}

TEST_CASE("String(size_t, char) < base buffer size", "[String]")
{
    FString s(12, 'c');

    REQUIRE(s[0] == 'c');
    REQUIRE(s[11] == 'c');
}

TEST_CASE("String(size_t, char) dynamic allocation", "[String]")
{
    FString s(24, 'd');

    REQUIRE(s[0] == 'd');
    REQUIRE(s[23] == 'd');
}

TEST_CASE("String(const char*, size_t) < base buffer size", "[String]")
{
    FString s("hello", 5);

    REQUIRE(s[0] == 'h');
    REQUIRE(s[4] == 'o');
}

TEST_CASE("String(const char*, size_t) dynamic allocation", "[String]")
{
    FString s("this is a much longer string", 28);

    REQUIRE(s[0] == 't');
    REQUIRE(s[27] == 'g');
}

TEST_CASE("String(const char*) < base buffer size", "[String]")
{
    FString s("hello");

    REQUIRE(s[0] == 'h');
    REQUIRE(s[4] == 'o');
}

TEST_CASE("String(const char*) dynamic allocation", "[String]")
{
    FString s("this is a much longer string");

    REQUIRE(s[0] == 't');
    REQUIRE(s[27] == 'g');
}

TEST_CASE("String(const char*, size_t, size_t) < base buffer size", "[String]")
{
    FString s("hello", 1, 2);

    REQUIRE(s[0] == 'e');
    REQUIRE(s[1] == 'l');
}

TEST_CASE("String(const char*, size_t, size_t) dynamic allocation", "[String]")
{
    FString s("this is a much longer string that requires dynamic allocation", 10, 25);

    REQUIRE(s[0] == 'm');
    REQUIRE(s[24] == 'r');
}

TEST_CASE("String() constructors for bool, char, ints, floats", "[String]")
{
    FString sbt(true);
    REQUIRE(sbt[0] == '1');

    FString sbf(false);
    REQUIRE(sbf[0] == '0');

    FString sc1('c');
    REQUIRE(sc1[0] == 'c');

    FString sc2('x');
    REQUIRE(sc2[0] == 'x');

    FString si321(42);
    REQUIRE(si321[0] == '4');
    REQUIRE(si321[1] == '2');

    FString si312(2147483647);
    REQUIRE(si312[0] == '2');
    REQUIRE(si312[9] == '7');

    FString si641(42ll);
    REQUIRE(si641[0] == '4');
    REQUIRE(si641[1] == '2');

    FString si642(9223372036854775807ll);
    REQUIRE(si642[0] == '9');
    REQUIRE(si642[18] == '7');

    FString sf321(42.0f);
    REQUIRE(sf321[0] == '4');
    REQUIRE(sf321[1] == '2');

    FString sf322(69.420f);
    REQUIRE(sf322[0] == '6');
    REQUIRE(sf322[1] == '9');
    REQUIRE(sf322[2] == '.');
    REQUIRE(sf322[3] == '4');

    FString sf641(-123.0);
    REQUIRE(sf641[0] == '-');
    REQUIRE(sf641[1] == '1');
    REQUIRE(sf641[2] == '2');
    REQUIRE(sf641[3] == '3');

    FString sf642(123.456);
    REQUIRE(sf642[0] == '1');
    REQUIRE(sf642[1] == '2');
    REQUIRE(sf642[2] == '3');
    REQUIRE(sf642[3] == '.');
    REQUIRE(sf642[4] == '4');
    REQUIRE(sf642[5] == '5');
    REQUIRE(sf642[6] == '6');
}

TEST_CASE("String::Append(char)", "[String]")
{
    FString s1("hello");
    REQUIRE(s1.Length() == 5);
    s1.Append('d');
    REQUIRE(s1.Length() == 6);
    REQUIRE(s1[5] == 'd');

    FString s2("long string is at least 32 chars");
    REQUIRE(s2.Length() == 32);
    s2.Append('!');
    REQUIRE(s2.Length() == 33);
    REQUIRE(s2[32] == '!');
}

TEST_CASE("String::Append(const char*)", "[String]")
{
    FString s1("small string");
    REQUIRE(s1.Length() == 12);
    s1.Append(" is longer");
    REQUIRE(s1.Length() == 22);
    REQUIRE(strcmp(s1.c_str(), "small string is longer") == 0);

    FString s2("slightly longer string that will");
    REQUIRE(s2.Length() == 32);
    s2.Append(" require more memory allocations");
    REQUIRE(s2.Length() == 64);
    REQUIRE(strcmp(s2.c_str(), "slightly longer string that will require more memory allocations") == 0);
}
