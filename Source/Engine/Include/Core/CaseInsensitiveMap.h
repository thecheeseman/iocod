// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CASE_INSENSITIVE_MAP_H
#define CORE_CASE_INSENSITIVE_MAP_H

#include <algorithm>
#include <cctype>
#include <Core/String.h>
#include <unordered_map>

namespace iocod {

namespace detail {
struct CaseInsensitive {
    struct Comparison {
        bool operator()(const String& lhs, const String& rhs) const
        {
            return std::ranges::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                      [](const unsigned char a, const unsigned char b) {
                                          return std::tolower(a) == std::tolower(b);
                                      });
        }
    };

    struct Hash {
        std::size_t operator()(String str) const
        {
            std::ranges::transform(str.begin(), str.end(), str.begin(), [](const unsigned char c) {
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
