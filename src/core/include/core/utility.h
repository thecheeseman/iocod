// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_UTILITY_H
#define CORE_UTILITY_H

/// Min, max, clamp, etc. macros or constexpr functions.
namespace iocod::Utility {

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
inline constexpr T KB(T x)
{
    return (x << 10);
}

template <typename T>
inline constexpr T MB(T x)
{
    return (x << 20);
}

template <typename T>
inline constexpr T GB(T x)
{
    return (x << 30);
}

template <typename T>
inline constexpr T TB(T x)
{
    return (x << 40);
}

} // namespace iocod::Utility

#endif // CORE_UTILITY_H
