// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/string_utilities.h>

#include <catch2/catch_test_macros.hpp>

using namespace iocod;

TEST_CASE("ToLower", "[StringUtilities]")
{
    REQUIRE(StringUtilities::ToLower("Hello World") == "hello world");
    REQUIRE(StringUtilities::ToLower("HELLO WORLD") == "hello world");
    REQUIRE(StringUtilities::ToLower("hELLO wORLD") == "hello world");
}

TEST_CASE("ToUpper", "[StringUtilities]")
{
    REQUIRE(StringUtilities::ToUpper("Hello World") == "HELLO WORLD");
    REQUIRE(StringUtilities::ToUpper("HELLO WORLD") == "HELLO WORLD");
    REQUIRE(StringUtilities::ToUpper("hELLO wORLD") == "HELLO WORLD");
}

TEST_CASE("Split", "[StringUtilities]")
{
    const String test1 = "comma,separated,string";
    const std::vector<String> expected1 = {"comma", "separated", "string"};
    REQUIRE(StringUtilities::Split(test1, ',') == expected1);

    const String test2 = "another,string.with;more/punctuation";
    const std::vector<String> expected2 = {"another", "string", "with", "more", "punctuation"};
    REQUIRE(StringUtilities::Split(test2, ",.;/") == expected2);
}

TEST_CASE("Tokenize", "[StringUtilities]")
{
    const String test = "This is a test string";
    const std::vector<String> expected = {"This", "is", "a", "test", "string"};
    REQUIRE(StringUtilities::Tokenize(test) == expected);
}

TEST_CASE("GetLines", "[StringUtilities]")
{
    const String test = "test\ntest2\ntest3";
    const std::vector<String> expected = {"test", "test2", "test3"};
    REQUIRE(StringUtilities::GetLines(test) == expected);
}

TEST_CASE("Replace", "[StringUtilities]")
{
    const String test1 = "test1 test2 test3";
    const String expected1 = "test1 test3 test3";
    REQUIRE(StringUtilities::Replace(test1, "test2", "test3") == expected1);

    const String test2 = "this is a longer test message";
    const String expected2 = "thas as a langar tast massaga";
    REQUIRE(StringUtilities::Replace(test2, "[aeiou]", "a") == expected2);

    const String test3 = "test\x15 with\x14 special\x01 characters";
    const String expected3 = "test with special characters";
    REQUIRE(StringUtilities::Replace(test3, "[^ -~]", "") == expected3);
}

TEST_CASE("Trim", "[StringUtilities]")
{
    const String test1 = "  test  ";
    const String expected1 = "test";
    REQUIRE(StringUtilities::TrimCopy(test1) == expected1);

    const String test2 = "test";
    const String expected2 = "test";
    REQUIRE(StringUtilities::TrimCopy(test2) == expected2);

    const String test3 = "  test";
    const String expected3 = "test";
    REQUIRE(StringUtilities::TrimCopy(test3) == expected3);

    const String test4 = "test  ";
    const String expected4 = "test";
    REQUIRE(StringUtilities::TrimCopy(test4) == expected4);
}

TEST_CASE("Escape", "[StringUtilities]")
{
    const String test1 = "test1\a\b\v\ntest2";
    const String expected1 = "test1\\a\\b\\v\\ntest2";
    REQUIRE(StringUtilities::Escape(test1) == expected1);

    const String test2 = "test1\\test2";
    const String expected2 = "test1\\\\test2";
    REQUIRE(StringUtilities::Escape(test2) == expected2);

    const String test3 = "test1\"test2 is an \'interesting\' test\"\n";
    const String expected3 = "test1\\\"test2 is an \\\'interesting\\\' test\\\"\\n";
    REQUIRE(StringUtilities::Escape(test3) == expected3);
}

TEST_CASE("Unescape", "[StringUtilities]")
{
    const String test1 = "test1\\a\\b\\v\\ntest2";
    const String expected1 = "test1\a\b\v\ntest2";
    REQUIRE(StringUtilities::Unescape(test1) == expected1);

    const String test2 = "test1\\\\test2";
    const String expected2 = "test1\\test2";
    REQUIRE(StringUtilities::Unescape(test2) == expected2);

    const String test3 = "test1\\\"test2 is an \\\'interesting\\\' test\\\"\\n";
    const String expected3 = "test1\"test2 is an \'interesting\' test\"\n";
    REQUIRE(StringUtilities::Unescape(test3) == expected3);
}

TEST_CASE("Clean", "[StringUtilities]")
{
    const String test1 = "test1\a\b\v\ntest2";
    const String expected1 = "test1test2";
    REQUIRE(StringUtilities::Clean(test1) == expected1);
}
