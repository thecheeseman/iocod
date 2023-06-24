// SPDX-FileCopyrightText: 2020 Daniil Dudkin
// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: MIT
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_FIXED_STRING_H
#define CORE_FIXED_STRING_H

#include <array>
#include <functional>
#include <iterator>
#include <ostream>
#include <string_view>
#include <type_traits>

// Modified from:
// https://github.com/unterumarmung/fixed_string/blob/master/include/fixed_string.hpp

namespace fixstr {

template <typename TChar, std::size_t N, typename TTraits = std::char_traits<TChar>>
struct basic_fixed_string {
    using storage_type = std::array<TChar, N + 1>;
    storage_type _data{};

    using traits_type = TTraits;
    using value_type = TChar;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;
    using reverse_iterator = typename storage_type::reverse_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using string_view_type = std::basic_string_view<value_type, traits_type>;
    static constexpr auto npos = string_view_type::npos;

    constexpr basic_fixed_string() noexcept = default;

    constexpr basic_fixed_string(const value_type (&array)[N + 1]) noexcept
    {
        std::copy(std::begin(array), std::end(array), _data.begin());
    }

    constexpr basic_fixed_string& operator=(const value_type (&array)[N + 1]) noexcept
    {
        std::copy(std::begin(array), std::end(array), _data.begin());
        return *this;
    }

    // iterators
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return _data.begin();
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return _data.begin();
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return _data.end() - 1;
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return _data.end() - 1;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return _data.cbegin();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return _data.cend() - 1;
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return _data.rbegin() + 1;
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return _data.rbegin() + 1;
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return _data.rend();
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return _data.rend();
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return _data.crbegin() + 1;
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return _data.crend();
    }

private:
    [[nodiscard]] constexpr static bool static_empty() noexcept
    {
        return N == 0;
    }

public:
    // capacity
    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return N;
    }

    [[nodiscard]] constexpr size_type length() const noexcept
    {
        return N;
    }

    [[nodiscard]] constexpr size_type max_size() const noexcept
    {
        return N;
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return static_empty();
    }

    // element access
    [[nodiscard]] constexpr reference operator[](size_type n)
    {
        return _data[n];
    }

    [[nodiscard]] constexpr const_reference operator[](size_type n) const
    {
        return _data[n];
    }

    [[nodiscard]] constexpr reference at(size_type n)
    {
        return _data.at(n);
    }

    [[nodiscard]] constexpr const_reference at(size_type n) const
    {
        return _data.at(n);
    }

    // The lack of C++20 concepts is disappointing
    // Basically what every `template<...>` line means is `requires (!empty())`
    template <typename..., bool NonEmpty = !static_empty(), typename = std::enable_if_t<NonEmpty>>
    [[nodiscard]] constexpr reference front() noexcept
    {
        return _data.front();
    }

    template <typename..., bool NonEmpty = !static_empty(), typename = std::enable_if_t<NonEmpty>>
    [[nodiscard]] constexpr const_reference front() const noexcept
    {
        return _data.front();
    }

    template <typename..., bool NonEmpty = !static_empty(), typename = std::enable_if_t<NonEmpty>>
    [[nodiscard]] constexpr reference back() noexcept
    {
        return _data[size() - 1];
    }

    template <typename..., bool NonEmpty = !static_empty(), typename = std::enable_if_t<NonEmpty>>
    [[nodiscard]] constexpr const_reference back() const noexcept
    {
        return _data[size() - 1];
    }

    [[nodiscard]] constexpr pointer data() noexcept
    {
        return _data.data();
    }

    [[nodiscard]] constexpr const_pointer data() const noexcept
    {
        return _data.data();
    }

    [[nodiscard]] constexpr const_pointer c_str() const noexcept
    {
        return data();
    }

private:
    template <size_t M>
    using same_with_other_size = basic_fixed_string<value_type, M, traits_type>;

    template <size_type pos, size_type count, size_type size>
    constexpr static size_type calculate_substr_size()
    {
        if constexpr (pos >= size)
            return 0;

        constexpr size_type rcount = std::min(count, size - pos);

        return rcount;
    }

    template <size_type pos, size_type count>
    using substr_result_type = same_with_other_size<calculate_substr_size<pos, count, N>()>;

public:
    // string operations
    [[nodiscard]] constexpr operator string_view_type()
        const noexcept // NOLINT(google-explicit-constructor)
    {
        return {data(), N};
    }

    template <size_type pos = 0, size_type count = npos, typename..., bool IsPosInBounds = pos <= N,
              typename = std::enable_if_t<IsPosInBounds>>
    [[nodiscard]] constexpr auto substr() const noexcept -> substr_result_type<pos, count>
    {
        substr_result_type<pos, count> result;
        std::copy(begin() + pos, begin() + pos + result.size(), result.begin());
        return result;
    }

    template <size_t M>
    [[nodiscard]] constexpr size_type find(const same_with_other_size<M>& str,
                                           size_type pos = 0) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find(str.sv(), pos);
    }

    [[nodiscard]] constexpr size_type find(string_view_type sv, size_type pos = 0) const noexcept
    {
        return sv().find(sv, pos);
    }

    [[nodiscard]] constexpr size_type find(const value_type* s, size_type pos, size_type n) const
    {
        return sv().find(s, pos, n);
    }

    [[nodiscard]] constexpr size_type find(const value_type* s, size_type pos = 0) const
    {
        return sv().find(s, pos);
    }

    [[nodiscard]] constexpr size_type find(value_type c, size_type pos = 0) const noexcept
    {
        return sv().find(c, pos);
    }

    template <size_t M>
    [[nodiscard]] constexpr size_type rfind(const same_with_other_size<M>& str,
                                            size_type pos = npos) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().rfind(str.sv(), pos);
    }

    [[nodiscard]] constexpr size_type rfind(string_view_type sv,
                                            size_type pos = npos) const noexcept
    {
        return sv().rfind(sv, pos);
    }

    [[nodiscard]] constexpr size_type rfind(const value_type* s, size_type pos, size_type n) const
    {
        return sv().rfind(s, pos, n);
    }

    [[nodiscard]] constexpr size_type rfind(const value_type* s, size_type pos = npos) const
    {
        return sv().rfind(s, pos);
    }

    [[nodiscard]] constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept
    {
        return sv().rfind(c, pos);
    }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_first_of(const same_with_other_size<M>& str,
                                                    size_type pos = 0) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_first_of(str.sv(), pos);
    }

    [[nodiscard]] constexpr size_type find_first_of(string_view_type sv,
                                                    size_type pos = 0) const noexcept
    {
        return sv().find_first_of(sv, pos);
    }

    [[nodiscard]] constexpr size_type find_first_of(const value_type* s, size_type pos,
                                                    size_type n) const
    {
        return sv().find_first_of(s, pos, n);
    }

    [[nodiscard]] constexpr size_type find_first_of(const value_type* s, size_type pos = 0) const
    {
        return sv().find_first_of(s, pos);
    }

    [[nodiscard]] constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept
    {
        return sv().find_first_of(c, pos);
    }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_last_of(const same_with_other_size<M>& str,
                                                   size_type pos = npos) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_last_of(str.sv(), pos);
    }

    [[nodiscard]] constexpr size_type find_last_of(string_view_type sv,
                                                   size_type pos = npos) const noexcept
    {
        return sv().find_last_of(sv, pos);
    }

    [[nodiscard]] constexpr size_type find_last_of(const value_type* s, size_type pos,
                                                   size_type n) const
    {
        return sv().find_last_of(s, pos, n);
    }

    [[nodiscard]] constexpr size_type find_last_of(const value_type* s, size_type pos = npos) const
    {
        return sv().find_last_of(s, pos);
    }

    [[nodiscard]] constexpr size_type find_last_of(value_type c,
                                                   size_type pos = npos) const noexcept
    {
        return sv().find_last_of(c, pos);
    }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_first_not_of(const same_with_other_size<M>& str,
                                                        size_type pos = 0) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_first_of(str.sv(), pos);
    }

    [[nodiscard]] constexpr size_type find_first_not_of(string_view_type sv,
                                                        size_type pos = 0) const noexcept
    {
        return sv().find_first_not_of(sv, pos);
    }

    [[nodiscard]] constexpr size_type find_first_not_of(const value_type* s, size_type pos,
                                                        size_type n) const
    {
        return sv().find_first_not_of(s, pos, n);
    }

    [[nodiscard]] constexpr size_type find_first_not_of(const value_type* s,
                                                        size_type pos = 0) const
    {
        return sv().find_first_not_of(s, pos);
    }

    [[nodiscard]] constexpr size_type find_first_not_of(value_type c,
                                                        size_type pos = 0) const noexcept
    {
        return sv().find_first_not_of(c, pos);
    }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_last_not_of(const same_with_other_size<M>& str,
                                                       size_type pos = npos) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_last_of(str.sv(), pos);
    }

    [[nodiscard]] constexpr size_type find_last_not_of(string_view_type sv,
                                                       size_type pos = npos) const noexcept
    {
        return sv().find_last_not_of(sv, pos);
    }

    [[nodiscard]] constexpr size_type find_last_not_of(const value_type* s, size_type pos,
                                                       size_type n) const
    {
        return sv().find_last_not_of(s, pos, n);
    }

    [[nodiscard]] constexpr size_type find_last_not_of(const value_type* s,
                                                       size_type pos = npos) const
    {
        return sv().find_last_not_of(s, pos);
    }

    [[nodiscard]] constexpr size_type find_last_not_of(value_type c,
                                                       size_type pos = npos) const noexcept
    {
        return sv().find_last_not_of(c, pos);
    }

    [[nodiscard]] constexpr int compare(string_view_type v) const noexcept
    {
        return sv().compare(v);
    }

    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, string_view_type v) const
    {
        return sv().compare(pos1, count1, v);
    }

    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, string_view_type v,
                                        size_type pos2, size_type count2) const
    {
        return sv().compare(pos1, count1, v, pos2, count2);
    }

    [[nodiscard]] constexpr int compare(const value_type* s) const
    {
        return sv().compare(s);
    }

    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type* s) const
    {
        return sv().compare(pos1, count1, s);
    }

    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type* s,
                                        size_type count2) const
    {
        return sv().compare(pos1, count1, s, count2);
    }

    [[nodiscard]] constexpr bool starts_with(string_view_type v) const noexcept
    {
        return sv().substr(0, v.size()) == v;
    }

    [[nodiscard]] constexpr bool starts_with(char c) const noexcept
    {
        return !empty() && traits_type::eq(front(), c);
    }

    [[nodiscard]] constexpr bool starts_with(const value_type* s) const noexcept
    {
        return starts_with(string_view_type(s));
    }

    [[nodiscard]] constexpr bool ends_with(string_view_type sv) const noexcept
    {
        return size() >= sv.size() && compare(size() - sv.size(), npos, sv) == 0;
    }

    [[nodiscard]] constexpr bool ends_with(value_type c) const noexcept
    {
        return !empty() && traits_type::eq(back(), c);
    }

    [[nodiscard]] constexpr bool ends_with(const value_type* s) const
    {
        return ends_with(string_view_type(s));
    }

    [[nodiscard]] constexpr bool contains(string_view_type sv) const noexcept
    {
        return find(sv) != npos;
    }

    [[nodiscard]] constexpr bool contains(value_type c) const noexcept
    {
        return find(c) != npos;
    }

    [[nodiscard]] constexpr bool contains(const value_type* s) const
    {
        return find(s) != npos;
    }

    void swap(basic_fixed_string& other) noexcept(std::is_nothrow_swappable_v<storage_type>)
    {
        _data.swap(other._data);
    }

private:
    constexpr string_view_type sv() const
    {
        return *this;
    }
};

template <typename TChar, typename TTraits, size_t N>
void swap(basic_fixed_string<TChar, N, TTraits>& lhs,
          basic_fixed_string<TChar, N, TTraits>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr bool operator==(const basic_fixed_string<TChar, M1, TTraits>& lhs,
                                        const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    if constexpr (M1 != M2)
        return false;
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) == rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator==(const basic_fixed_string<TChar, N, TTraits>& lhs,
                                        std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) == rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator==(std::basic_string_view<TChar, TTraits> lhs,
                                        const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs == static_cast<sv_type>(rhs);
}

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr auto operator<=>(const basic_fixed_string<TChar, M1, TTraits>& lhs,
                                         const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) <=> rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr auto operator<=>(const basic_fixed_string<TChar, N, TTraits>& lhs,
                                         std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) <=> rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr auto operator<=>(std::basic_string_view<TChar, TTraits> lhs,
                                         const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs <=> static_cast<sv_type>(rhs);
}

template <typename TChar, size_t N>
basic_fixed_string(const TChar (&)[N]) -> basic_fixed_string<TChar, N - 1>;

// Early GCC versions that support cNTTP were not able to deduce size_t parameter
// of basic_fixed_string when fixed_string and other typedef were just type aliases.
// That's why the following code is written in this way.
template <size_t N>
struct fixed_string : basic_fixed_string<char, N> {
    using basic_fixed_string<char, N>::basic_fixed_string;
};

template <std::size_t N>
fixed_string(const char (&)[N]) -> fixed_string<N - 1>;

template <size_t N>
struct fixed_u8string : basic_fixed_string<char8_t, N> {
    using basic_fixed_string<char8_t, N>::basic_fixed_string;
};

template <std::size_t N>
fixed_u8string(const char8_t (&)[N]) -> fixed_u8string<N - 1>;

template <size_t N>
struct fixed_u16string : basic_fixed_string<char16_t, N> {
    using basic_fixed_string<char16_t, N>::basic_fixed_string;
};

template <std::size_t N>
fixed_u16string(const char16_t (&)[N]) -> fixed_u16string<N - 1>;

template <size_t N>
struct fixed_u32string : basic_fixed_string<char32_t, N> {
    using basic_fixed_string<char32_t, N>::basic_fixed_string;
};

template <std::size_t N>
fixed_u32string(const char32_t (&)[N]) -> fixed_u32string<N - 1>;

template <size_t N>
struct fixed_wstring : basic_fixed_string<wchar_t, N> {
    using basic_fixed_string<wchar_t, N>::basic_fixed_string;
};

template <std::size_t N>
fixed_wstring(const wchar_t (&)[N]) -> fixed_wstring<N - 1>;

template <typename TChar, size_t N, size_t M, typename TTraits>
constexpr basic_fixed_string<TChar, N + M, TTraits> operator+(
    const basic_fixed_string<TChar, N, TTraits>& lhs,
    const basic_fixed_string<TChar, M, TTraits>& rhs)
{
    basic_fixed_string<TChar, N + M, TTraits> result;
    std::copy(lhs.begin(), lhs.end(), result.begin());
    std::copy(rhs.begin(), rhs.end(), result.begin() + N);
    return result;
}

template <typename TChar, size_t N, size_t M, typename TTraits>
constexpr basic_fixed_string<TChar, N - 1 + M, TTraits> operator+(
    const TChar (&lhs)[N], const basic_fixed_string<TChar, M, TTraits>& rhs)
{
    basic_fixed_string lhs2 = lhs;
    return lhs2 + rhs;
}

template <typename TChar, size_t N, size_t M, typename TTraits>
constexpr basic_fixed_string<TChar, N + M - 1, TTraits> operator+(
    const basic_fixed_string<TChar, N, TTraits>& lhs, const TChar (&rhs)[M])
{
    basic_fixed_string rhs2 = rhs;
    return lhs + rhs2;
}

namespace details {
template <typename TChar>
constexpr basic_fixed_string<TChar, 1> from_char(TChar ch)
{
    basic_fixed_string<TChar, 1> fs;
    fs[0] = ch;
    return fs;
}
} // namespace details

template <typename TChar, size_t N, typename TTraits>
constexpr basic_fixed_string<TChar, N + 1, TTraits> operator+(
    TChar lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    return details::from_char(lhs) + rhs;
}

template <typename TChar, size_t N, typename TTraits>
constexpr basic_fixed_string<TChar, N + 1, TTraits> operator+(
    const basic_fixed_string<TChar, N, TTraits>& lhs, TChar rhs)
{
    return lhs + details::from_char(rhs);
}

template <typename TChar, size_t N, typename TTraits>
std::basic_ostream<TChar, TTraits>& operator<<(std::basic_ostream<TChar, TTraits>& out,
                                               const basic_fixed_string<TChar, N, TTraits>& str)
{
    out << str.data();
    return out;
}

} // namespace fixstr

// hash support
namespace std {
template <size_t N>
struct hash<fixstr::fixed_string<N>> {
    using argument_type = fixstr::fixed_string<N>;

    size_t operator()(const argument_type& str) const
    {
        using sv_t = typename argument_type::string_view_type;
        return std::hash<sv_t>()(static_cast<sv_t>(str));
    }
};

template <size_t N>
struct hash<fixstr::fixed_u8string<N>> {
    using argument_type = fixstr::fixed_u8string<N>;

    size_t operator()(const argument_type& str) const
    {
        using sv_t = typename argument_type::string_view_type;
        return std::hash<sv_t>()(static_cast<sv_t>(str));
    }
};

template <size_t N>
struct hash<fixstr::fixed_u16string<N>> {
    using argument_type = fixstr::fixed_u16string<N>;

    size_t operator()(const argument_type& str) const
    {
        using sv_t = typename argument_type::string_view_type;
        return std::hash<sv_t>()(static_cast<sv_t>(str));
    }
};

template <size_t N>
struct hash<fixstr::fixed_u32string<N>> {
    using argument_type = fixstr::fixed_u32string<N>;

    size_t operator()(const argument_type& str) const
    {
        using sv_t = typename argument_type::string_view_type;
        return std::hash<sv_t>()(static_cast<sv_t>(str));
    }
};

template <size_t N>
struct hash<fixstr::fixed_wstring<N>> {
    using argument_type = fixstr::fixed_wstring<N>;

    size_t operator()(const argument_type& str) const
    {
        using sv_t = typename argument_type::string_view_type;
        return std::hash<sv_t>()(static_cast<sv_t>(str));
    }
};

} // namespace std

namespace iocod {

template <size_t N>
using FixedString = fixstr::fixed_string<N>;

}

#endif // CORE_FIXED_STRING_H
