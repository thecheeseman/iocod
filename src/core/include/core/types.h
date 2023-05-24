// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <stddef.h>
#include <stdint.h>
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

// useful filesize literals
inline consteval auto operator"" _KB(u64 value) -> u64
{
    return value * 1024;
}

inline consteval auto operator"" _MB(u64 value) -> u64
{
    return value * 1024 * 1024;
}

inline consteval auto operator"" _GB(u64 value) -> u64
{
    return value * 1024 * 1024 * 1024;
}

// other useful utilities
template <typename T>
inline constexpr T Min(T a, T b)
{
    return (a < b) ? a : b;
}

template <typename T>
inline constexpr T Max(T a, T b)
{
    return (a > b) ? a : b;
}

template <typename T>
inline constexpr T Clamp(T a, T x, T b)
{
    return (x < a) ? a : (b < x) ? b : x;
}

template <typename T>
inline constexpr T Pow2AlignUp(T x, T p)
{
    return ((x + (p - 1)) & ~(p - 1));
}

template <typename T>
inline constexpr T Pow2AlignDown(T x, T p)
{
    return (x & ~(p - 1));
}

template <typename T>
inline constexpr T BytesToKB(T x)
{
    return (x >> 10);
}

template <typename T>
inline constexpr T BytesToMB(T x)
{
    return (x >> 20);
}

template <typename T>
inline constexpr T BytesToGB(T x)
{
    return (x >> 30);
}

template <typename T>
inline constexpr T BytesToTB(T x)
{
    return (x >> 40);
}

inline consteval u8 Bit(u8 n)
{
    return (1 << n);
}

} // namespace iocod

#endif // CORE_TYPES_H
