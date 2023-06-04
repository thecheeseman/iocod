// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace iocod {

using String = std::string;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

// q3 types
using vec_t = float;
using vec2_t = vec_t[2];
using vec3_t = vec_t[3];
using vec4_t = vec_t[4];

inline constexpr u64 KB = 1000ull;
inline constexpr u64 KiB = 1024ull;
inline constexpr u64 MB = 1000ull * KB;
inline constexpr u64 MiB = 1024ull * KiB;
inline constexpr u64 GB = 1000ull * MB;
inline constexpr u64 GiB = 1024ull * MiB;
inline constexpr u64 TB = 1000ull * GB;
inline constexpr u64 TiB = 1024ull * GiB;

consteval u64 operator"" _KB(u64 value)
{
    return value * KB;
}

consteval u64 operator"" _KiB(u64 value)
{
    return value * KiB;
}

consteval u64 operator"" _MB(u64 value)
{
    return value * MB;
}

consteval u64 operator"" _MiB(u64 value)
{
    return value * MiB;
}

consteval u64 operator"" _GB(u64 value)
{
    return value * GB;
}

consteval u64 operator"" _GiB(u64 value)
{
    return value * GiB;
}

template <typename T>
constexpr T Min(T a, T b)
{
    return (a < b) ? a : b;
}

template <typename T>
constexpr T Max(T a, T b)
{
    return (a > b) ? a : b;
}

template <typename T>
constexpr T Clamp(T a, T x, T b)
{
    return (x < a) ? a : (b < x) ? b : x;
}

template <typename T>
constexpr T Pow2AlignUp(T x, T p)
{
    return ((x + (p - 1)) & ~(p - 1));
}

template <typename T>
constexpr T Pow2AlignDown(T x, T p)
{
    return (x & ~(p - 1));
}

template <typename T>
constexpr T BytesToKB(T x)
{
    return (x >> 10);
}

template <typename T>
constexpr T BytesToMB(T x)
{
    return (x >> 20);
}

template <typename T>
constexpr T BytesToGB(T x)
{
    return (x >> 30);
}

template <typename T>
constexpr T BytesToTB(T x)
{
    return (x >> 40);
}

consteval u8 Bit(u8 n)
{
    return (1 << n);
}

} // namespace iocod

#endif // CORE_TYPES_H
