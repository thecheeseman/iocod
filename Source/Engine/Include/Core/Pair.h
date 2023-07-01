// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_CORE_PAIR_H
#define ENGINE_CORE_PAIR_H

namespace iocod {

template <typename F, typename S>
struct Pair {
    F first;
    S second;

    Pair() : first(), second() {}
    Pair(const F& first, const S& second) : first(first), second(second) {}
};

template <typename F, typename S>
bool operator==(const Pair<F, S>& lhs, const Pair<F, S>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename F, typename S>
bool operator!=(const Pair<F, S>& lhs, const Pair<F, S>& rhs)
{
    return !(lhs == rhs);
}

template <typename F, typename S>
struct PairSort {
    bool operator()(const Pair<F, S>& lhs, const Pair<F, S>& rhs) const
    {
        if (lhs.first != rhs.first)
            return lhs.first < rhs.first;

        return lhs.second < rhs.second;
    }
};

} // namespace iocod

#endif //ENGINE_CORE_PAIR_H
