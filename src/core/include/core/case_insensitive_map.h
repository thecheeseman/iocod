// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CASE_INSENSITIVE_MAP_H
#define CORE_CASE_INSENSITIVE_MAP_H

#include <core/types.h>

#include <algorithm>
#include <cctype>
#include <unordered_map>

namespace iocod {

namespace detail {
struct CaseInsensitive {
    struct Comparison {
        bool operator()(const String& lhs, const String& rhs) const
        {
            return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                              [](unsigned char a, unsigned char b) {
                                  return std::tolower(a) == std::tolower(b);
                              });
        }
    };

    struct Hash {
        std::size_t operator()(String str) const
        {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
                return static_cast<unsigned char>(std::tolower(c));
            });
            return std::hash<String>{}(str);
        }
    };
};
} // namespace detail

template <typename T, typename Key = String>
using CaseInsensitiveMap =
    std::unordered_map<Key, T, detail::CaseInsensitive::Hash, detail::CaseInsensitive::Comparison>;

} // namespace iocod

#endif // CORE_CASE_INSENSITIVE_MAP_H
