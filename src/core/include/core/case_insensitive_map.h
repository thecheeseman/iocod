// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CASE_INSENSITIVE_MAP_H
#define CORE_CASE_INSENSITIVE_MAP_H

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

namespace iocod {

namespace detail {
struct case_insensitive {
    struct comparison {
        bool operator()(const std::string& lhs, const std::string& rhs) const
        {
            return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                              [](unsigned char a, unsigned char b) {
                                  return std::tolower(a) == std::tolower(b);
                              });
        }
    };

    struct hash {
        std::size_t operator()(std::string str) const
        {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
                return static_cast<unsigned char>(std::tolower(c));
            });
            return std::hash<std::string>{}(str);
        }
    };
};
} // namespace detail

template <typename T, typename Key = std::string>
using case_insensitive_map = std::unordered_map<Key, T, detail::case_insensitive::hash,
                                                detail::case_insensitive::comparison>;

} // namespace iocod

#endif // CORE_CASE_INSENSITIVE_MAP_H
