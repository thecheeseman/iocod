// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <stdint.h>
#include <stddef.h>

namespace iocod {

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

// q3 types
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

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

} // namespace iocod

#endif // CORE_TYPES_H
