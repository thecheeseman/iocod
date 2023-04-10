// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_UTILITY_H
#define CORE_UTILITY_H

/// Min, max, clamp, etc. macros or constexpr functions.
#ifdef __cplusplus
namespace Utility {
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

    #define MIN(a, b)      Utility::Min(a, b)
    #define MAX(a, b)      Utility::Max(a, b)
    #define CLAMP(a, x, b) Utility::Clamp(a, x, b)
} // namespace Utility
#else
    #define MIN(a, b)      (((a) < (b)) ? (a) : (b))
    #define MAX(a, b)      (((a) > (b)) ? (a) : (b))
    #define CLAMP(a, x, b) (((x) < (a)) ? (a) : ((b) < (x)) ? (b) : (x))
#endif

#define CLAMP_TOP(a, b)    MIN(a, b)
#define CLAMP_BOTTOM(a, b) MAX(a, b)

/// Alignment macros or constexpr functions.
#ifdef __cplusplus
namespace Utility {
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
} // namespace utility

    #define POW2_ALIGN_UP(x, p)   Utility::Pow2AlignUp(x, p)
    #define POW2_ALIGN_DOWN(x, p) Utility::Pow2AlignDown(x, p)
#else
    #define POW2_ALIGN_UP(x, p)   (((x) + ((p) -1)) & ~((p) -1))
    #define POW2_ALIGN_DOWN(x, p) ((x) & ~((p) -1))
#endif

/// File size macros or constexpr functions.
#ifdef __cplusplus
namespace Utility {
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
} // namespace Utility

    #define KB(x) Utility::KB(x)
    #define MB(x) Utility::MB(x)
    #define GB(x) Utility::GB(x)
    #define TB(x) Utility::TB(x)
#else
    #define KB(x) ((x) << 10)
    #define MB(x) ((x) << 20)
    #define GB(x) ((x) << 30)
    #define TB(x) ((x) << 40)
#endif

#endif // CORE_UTILITY_H
