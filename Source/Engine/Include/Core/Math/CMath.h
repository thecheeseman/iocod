// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_CORE_MATH_CMATH_H
#define ENGINE_CORE_MATH_CMATH_H

#include <type_traits>

namespace iocod {

template <typename IntType, std::enable_if_t<std::is_integral_v<IntType> && std::is_signed_v<IntType>, bool> = true>
constexpr IntType Abs(IntType i)
{
    return i < 0 ? -i : i;
}

template <typename UIntType, std::enable_if_t<std::is_integral_v<UIntType> && std::is_unsigned_v<UIntType>, bool> = true>
constexpr UIntType Abs(UIntType u)
{
    return u;
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

} // namespace iocod

#endif // ENGINE_CORE_MATH_CMATH_H
