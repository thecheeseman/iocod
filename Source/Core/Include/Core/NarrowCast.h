// SPDX-FileCopyrightText: 2015 Microsoft Corporation
// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: MIT
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_NARROW_CAST_H
#define CORE_NARROW_CAST_H

#include <exception>

// Borrowed from GSL

namespace iocod {

template <typename T, typename U>
constexpr T NarrowCast(U&& u) noexcept
{
    return static_cast<T>(std::forward<U>(u));
}

struct NarrowingError : std::exception {
    [[nodiscard]] const char* what() const noexcept override
    {
        return "narrowing error";
    }
};

template <typename T, typename U, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
constexpr T Narrow(U u)
{
    constexpr bool isDifferentSignedness = std::is_signed_v<T> != std::is_signed_v<U>;
    const T t = NarrowCast<T>(u);

    if (static_cast<U>(t) != u || (isDifferentSignedness && ((t < T{}) != (u < U{}))))
        throw NarrowingError{};

    return t;
}

template <typename T, typename U, typename std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
constexpr T Narrow(U u)
{
    const T t = NarrowCast<T>(u);

    if (static_cast<U>(t) != u)
        throw NarrowingError{};

    return t;
}

} // namespace iocod

#endif // CORE_NARROW_CAST_H
