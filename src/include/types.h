#pragma once

#include <cstdint>

namespace iocod {
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8f = int_fast8_t;
using i16f = int_fast16_t;
using i32f = int_fast32_t;
using i64f = int_fast64_t;
using u8f = uint_fast8_t;
using u16f = uint_fast16_t;
using u32f = uint_fast32_t;
using u64f = uint_fast64_t;

// c++23 has float16_t, float32_t, etc
using f32 = float;
using f64 = double;
using f128 = long double;

template <typename T>
inline T min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template <typename T>
inline T max(const T& a, const T& b)
{
    return a > b ? a : b;
}

template <typename T>
inline T clamp(T val, T min_, T max_)
{
    return min(max(min_, val), max_);
}
}; // namespace iocod
