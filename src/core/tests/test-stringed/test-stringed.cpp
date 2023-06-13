// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/stringed.h>
#include <catch2/catch_test_macros.hpp>

using namespace iocod;

TEST_CASE("default constructor", "stringed")
{
    StringReference s;

    REQUIRE(s.DefaultString() == nullptr);
}

TEST_CASE("construct with args", "stringed")
{
    StringReference s("OBJ_PROVIDEDISTRACTION", "test");

    REQUIRE(strcmp(s.Reference(), "OBJ_PROVIDEDISTRACTION") == 0);
    REQUIRE(strcmp(s.DefaultString(), "test") == 0);
}

TEST_CASE("construct alternate", "stringed")
{
    StringReference s("TEST_COLOR", "color", StringLanguage::British, "colour");

    REQUIRE(strcmp(s.DefaultString(), "color") == 0);
    REQUIRE(strcmp(s.GetString(StringLanguage::British), "colour") == 0);
}

TEST_CASE("duplicate alternate languages", "stringed")
{
    StringReference s("TEST_COLOR", "color", StringLanguage::British, "colour");

    REQUIRE_THROWS(s.AddAlternate(StringLanguage::British, "colour"));
}
