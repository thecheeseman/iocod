// SPDX-FileCopyrightText: 2015 Microsoft Corporation
// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: MIT
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_NARROW_CAST_HPP
#define CORE_NARROW_CAST_HPP

#include <exception>

// Borrowed from GSL

namespace iocod {

template <typename T, typename U>
constexpr T narrow_cast(U&& u) noexcept
{
    return static_cast<T>(std::forward<U>(u));
}

struct narrowing_error : public std::exception {
    const char* what() const noexcept override
    {
        return "narrowing error";
    }
};

template <typename T, typename U, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
constexpr T narrow(U u)
{
    constexpr const bool is_different_signedness = std::is_signed_v<T> != std::is_signed_v<U>;
    const T t = narrow_cast<T>(u);

    if (static_cast<U>(t) != u || (is_different_signedness && ((t < T{}) != (u < U{}))))
        throw narrowing_error{};

    return t;
}

template <typename T, typename U, typename std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
constexpr T narrow(U u)
{
    const T t = narrow_cast<T>(u);

    if (static_cast<U>(t) != u)
        throw narrowing_error{};

    return t;
}

} // namespace iocod

#endif // CORE_NARROW_CAST_HPP
