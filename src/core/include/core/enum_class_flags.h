// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_ENUM_CLASS_FLAGS_H
#define CORE_ENUM_CLASS_FLAGS_H

#include <type_traits>

namespace iocod {

template <typename E>
struct EnumFlag {
    static constexpr bool enabled = false;
};

template <typename E>
typename std::enable_if_t<EnumFlag<E>::enabled, E> operator|(E lhs, E rhs)
{
    using ut = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<ut>(lhs) | static_cast<ut>(rhs));
}

template <typename E>
typename std::enable_if_t<EnumFlag<E>::enabled, E> operator&(E lhs, E rhs)
{
    using ut = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<ut>(lhs) & static_cast<ut>(rhs));
}

template <typename E>
typename std::enable_if_t<EnumFlag<E>::enabled, E> operator^(E lhs, E rhs)
{
    using ut = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<ut>(lhs) ^ static_cast<ut>(rhs));
}

template <typename E>
typename std::enable_if_t<EnumFlag<E>::enabled, E> operator~(E rhs)
{
    using ut = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<ut>(rhs));
}

} // namespace iocod

#endif // CORE_ENUM_CLASS_FLAGS_H
