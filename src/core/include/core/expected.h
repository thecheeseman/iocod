// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_EXPECTED_H
#define CORE_EXPECTED_H

#include <nonstd/expected.hpp>

namespace iocod {

template <typename T, typename E>
using Expected = nonstd::expected<T, E>;

template <typename E>
using Unexpected = nonstd::unexpected<E>;

template <typename E>
constexpr Unexpected<std::decay_t<E>> MakeUnexpected(E&& value)
{
    return Unexpected<std::decay_t<E>>(std::forward<E>(value));
}

} // namespace iocod

#endif // CORE_EXPECTED_H
