// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <doctest/doctest.h>

using namespace iocod;

TEST_CASE("static_assert")
{
    static_assert(ByteSwap<int8_t>(0x12) == 0x12);
    static_assert(ByteSwap<int16_t>(0x1234) == 0x3412);
    static_assert(ByteSwap<int32_t>(0x12345678) == 0x78563412);
    static_assert(ByteSwap<int64_t>(0x123456789abcdef0) == static_cast<int64_t>(0xf0debc9a78563412));
    static_assert(ByteSwap<uint8_t>(0x21) == 0x21);
    static_assert(ByteSwap<uint16_t>(0x4321) == 0x2143);
    static_assert(ByteSwap<uint32_t>(0x87654321) == 0x21436587);
    static_assert(ByteSwap<uint64_t>(0xfedcba9876543210) == static_cast<uint64_t>(0x1032547698badcfe));
    static_assert(ByteSwap<const uint32_t>(0xdeadbeef) == 0xefbeadde);
    //static_assert(ByteSwap<volatile uint32_t>(0xdeadbeef) == 0xefbeadde);
    static_assert(ByteSwap<int32_t>(0xdeadbeef) == static_cast<int32_t>(0xefbeadde));

    volatile int8_t a = 0x12;
    volatile int16_t b = 0x1234;
    volatile int32_t c = 0x12345678;
    volatile int64_t d = 0x123456789abcdef0;
    volatile uint8_t e = 0x21;
    volatile uint16_t f = 0x4321;
    volatile uint32_t g = 0x87654321;
    volatile uint64_t h = 0xfedcba9876543210;
    REQUIRE(ByteSwap<int8_t>(a) == 0x12);
    REQUIRE(ByteSwap<int16_t>(b) == 0x3412);
    REQUIRE(ByteSwap(c) == 0x78563412);
    REQUIRE(ByteSwap(d) == 0xf0debc9a78563412);
    REQUIRE(ByteSwap<uint8_t>(e) == 0x21);
    REQUIRE(ByteSwap<uint16_t>(f) == 0x2143);
    REQUIRE(ByteSwap(g) == 0x21436587);
    REQUIRE(ByteSwap(h) == 0x1032547698badcfe);
    REQUIRE(ByteSwap(ByteSwap<int8_t>(a)) == a);
    REQUIRE(ByteSwap(ByteSwap<int16_t>(b)) == b);
    REQUIRE(ByteSwap(ByteSwap(c)) == c);
    REQUIRE(ByteSwap(ByteSwap(d)) == d);
    REQUIRE(ByteSwap(ByteSwap<uint8_t>(e)) == e);
    REQUIRE(ByteSwap(ByteSwap<uint16_t>(f)) == f);
    REQUIRE(ByteSwap(ByteSwap(g)) == g);
    REQUIRE(ByteSwap(ByteSwap(h)) == h);
    REQUIRE(ByteSwap<const uint32_t>(0xdeadbeef) == 0xefbeadde);
    REQUIRE(ByteSwap<volatile uint32_t>(0xdeadbeef) == 0xefbeadde);
}
