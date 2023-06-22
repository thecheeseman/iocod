// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <..\Include\Core\Vector.h>

#include <catch2/catch_test_macros.hpp>

using namespace iocod;

TEST_CASE("empty constructor", "[Vec3]")
{
    Vec3 v1;

    REQUIRE(!v1.IsValid());
}

TEST_CASE("single value constructor", "[Vec3]")
{
    Vec3 v1{1.0f};
    REQUIRE(v1.x == 1.0f);
    REQUIRE(v1.y == 1.0f);
    REQUIRE(v1.z == 1.0f);
}

TEST_CASE("multi value constructor", "[Vec3]")
{
    Vec3 v1{1.0f, 2.0f, 3.0f};
    REQUIRE(v1.x == 1.0f);
    REQUIRE(v1.y == 2.0f);
    REQUIRE(v1.z == 3.0f);
}

TEST_CASE("copy constructor", "[Vec3]")
{
    Vec3 v1{1.0f, 2.0f, 3.0f};
    Vec3 v2{v1};
    REQUIRE(v2.x == 1.0f);
    REQUIRE(v2.y == 2.0f);
    REQUIRE(v2.z == 3.0f);
}

TEST_CASE("copy assignment", "[Vec3]")
{
    Vec3 v1{1.0f, 2.0f, 3.0f};
    Vec3 v2;
    v2 = v1;
    REQUIRE(v2.x == 1.0f);
    REQUIRE(v2.y == 2.0f);
    REQUIRE(v2.z == 3.0f);
}

TEST_CASE("move constructor", "[Vec3]")
{
    Vec3 v1{1.0f, 2.0f, 3.0f};
    Vec3 v2{std::move(v1)};
    REQUIRE(v2.x == 1.0f);
    REQUIRE(v2.y == 2.0f);
    REQUIRE(v2.z == 3.0f);
}

TEST_CASE("move assignment", "[Vec3]")
{
    Vec3 v1{1.0f, 2.0f, 3.0f};
    Vec3 v2;
    v2 = std::move(v1);
    REQUIRE(v2.x == 1.0f);
    REQUIRE(v2.y == 2.0f);
    REQUIRE(v2.z == 3.0f);
}

TEST_CASE("compare", "[Vec3]")
{
    Vec3 v1{1.0f, 2.0f, 3.0f};
    Vec3 v2{1.0f, 2.0f, 3.0f};
    Vec3 v3{2.0f, 4.0f, 6.0f};
    REQUIRE(v1 == v2);
    REQUIRE(v1 != v3);
}

TEST_CASE("zero", "[Vec3]")
{
    Vec3 v1{1.0f, 2.0f, 3.0f};
    v1.Zero();
    REQUIRE(v1.x == 0.0f);
    REQUIRE(v1.y == 0.0f);
    REQUIRE(v1.z == 0.0f);
}

TEST_CASE("length", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    REQUIRE(std::fabs(v1.Length() - 7.48331f) < 0.00001f);
}

TEST_CASE("length squared", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    REQUIRE(v1.LengthSquared() == 56.0f);
}

TEST_CASE("normalize", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    v1.Normalize();
    REQUIRE(std::fabs(v1.x - 0.267261f) < 0.00001f);
    REQUIRE(std::fabs(v1.y - 0.534522f) < 0.00001f);
    REQUIRE(std::fabs(v1.z - 0.801783f) < 0.00001f);
}

TEST_CASE("addition", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    Vec3 v2{1.0f, 2.0f, 3.0f};
    Vec3 v3 = v1 + v2;
    REQUIRE(v3.x == 3.0f);
    REQUIRE(v3.y == 6.0f);
    REQUIRE(v3.z == 9.0f);
}

TEST_CASE("subtraction", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    Vec3 v2{1.0f, 2.0f, 3.0f};
    Vec3 v3 = v1 - v2;
    REQUIRE(v3.x == 1.0f);
    REQUIRE(v3.y == 2.0f);
    REQUIRE(v3.z == 3.0f);
}

TEST_CASE("multiplication", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    Vec3 v2{1.0f, 2.0f, 3.0f};
    Vec3 v3 = v1 * v2;
    REQUIRE(v3.x == 2.0f);
    REQUIRE(v3.y == 8.0f);
    REQUIRE(v3.z == 18.0f);
}

TEST_CASE("division", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    Vec3 v2{1.0f, 2.0f, 3.0f};
    Vec3 v3 = v1 / v2;
    REQUIRE(v3.x == 2.0f);
    REQUIRE(v3.y == 2.0f);
    REQUIRE(v3.z == 2.0f);
}

TEST_CASE("scalar multiplication", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    Vec3 v2 = v1 * 2.0f;
    REQUIRE(v2.x == 4.0f);
    REQUIRE(v2.y == 8.0f);
    REQUIRE(v2.z == 12.0f);
}

TEST_CASE("scalar division", "[Vec3]")
{
    Vec3 v1{2.0f, 4.0f, 6.0f};
    Vec3 v2 = v1 / 2.0f;
    REQUIRE(v2.x == 1.0f);
    REQUIRE(v2.y == 2.0f);
    REQUIRE(v2.z == 3.0f);
}

TEST_CASE("addition assignment", "[Vec3]")
{
    Vec3 v{40.f, 20.f, 10.f};
    v += Vec3{1.f, 2.f, 3.f};
    REQUIRE(v.x == 41.f);
    REQUIRE(v.y == 22.f);
    REQUIRE(v.z == 13.f);
}

TEST_CASE("subtraction assignment", "[Vec3]")
{
    Vec3 v{40.f, 20.f, 10.f};
    v -= Vec3{1.f, 2.f, 3.f};
    REQUIRE(v.x == 39.f);
    REQUIRE(v.y == 18.f);
    REQUIRE(v.z == 7.f);
}

TEST_CASE("multiplication assignment", "[Vec3]")
{
    Vec3 v{40.f, 20.f, 10.f};
    v *= Vec3{1.f, 2.f, 3.f};
    REQUIRE(v.x == 40.f);
    REQUIRE(v.y == 40.f);
    REQUIRE(v.z == 30.f);
}

TEST_CASE("division assignment", "[Vec3]")
{
    Vec3 v{40.f, 20.f, 10.f};
    v /= Vec3{1.f, 2.f, 1.f};
    REQUIRE(v.x == 40.f);
    REQUIRE(v.y == 10.f);
    REQUIRE(v.z == 10.f);
}
