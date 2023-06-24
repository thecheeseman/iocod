// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <..\Include\Core\Version.h>

#include <catch2/catch_test_macros.hpp>

using namespace iocod;

TEST_CASE("version", "version")
{
    constexpr auto version = VersionEncode(15, 275, 325);
    REQUIRE(VersionDecodeMajor(version) == 15);
    REQUIRE(VersionDecodeMinor(version) == 275);
    REQUIRE(VersionDecodePatch(version) == 325);

    constexpr auto iocodVersion = VersionEncode(kVersionMajor, kVersionMinor, kVersionPatch);
    REQUIRE(VersionDecodeMajor(iocodVersion) == kVersionMajor);
    REQUIRE(VersionDecodeMinor(iocodVersion) == kVersionMinor);
    REQUIRE(VersionDecodePatch(iocodVersion) == kVersionPatch);
}
