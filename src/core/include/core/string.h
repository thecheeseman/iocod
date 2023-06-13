// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRING_H
#define CORE_STRING_H

#include <core/platform.h>

namespace iocod {

template <typename T>
struct is_char_type : std::false_type {};

template <>
struct is_char_type<char> : std::true_type {};

template <>
struct is_char_type<wchar_t> : std::true_type {};

template <>
struct is_char_type<char8_t> : std::true_type {};

template <>
struct is_char_type<char16_t> : std::true_type {};

template <>
struct is_char_type<char32_t> : std::true_type {};

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strcpy(CharT* dest, const CharT* src)
{
    const CharT* s = src;
    CharT* d = dest;

    while ((*d++ = *s++) != 0) {}

    return dest;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strncpy(CharT* dest, const CharT* src, size_t n)
{
    const CharT* s = src;
    CharT* d = dest;

    n++;
    while (--n) {
        if ((*d++ = *s++) == 0) {
            while (--n)
                *d++ = 0;

            break;
        }
    }

    return dest;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strcat(CharT* dest, const CharT* src)
{
    const CharT* s = src;
    CharT* d = dest;

    while (*d++) {}
    d--;
    while ((*d++ = *s++) != 0) {}

    return dest;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strncat(CharT* dest, const CharT* src, size_t n)
{
    const CharT* s = src;
    CharT* d = dest;

    while (*d++) {}
    --d;
    ++n;
    while (--n) {
        if ((*d++ = *s++) == 0) {
            --d;
            break;
        }
    }
    *d = 0;

    return dest;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr size_t Strlen(const CharT* str)
{
#if IOCOD_HAS_BUILTIN(__builtin_strlen)
    // don't think any __builtin_strlen implementations support other char types
    if constexpr (std::is_same_v<CharT, char>)
        return __builtin_strlen(str);
#endif

    size_t len = 0;

    do {
        ++len;
    } while (*str++);

    return len - 1;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strend(const CharT* str)
{
    while (*str)
        ++str;

    return const_cast<CharT*>(str);
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strdup(const CharT* str)
{
    // TODO: custom memory allocators
    if (!str)
        return nullptr;

    const size_t len = Strlen(str);
    char *const dup = new char[len + 1];

    Strcpy(dup, str);
    return dup;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strdel(const CharT* str)
{
    // TODO: custom memory allocators
    delete[] str;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strchr(const CharT* str, CharT c)
{
    do {
        if (*str == c)
            return const_cast<CharT*>(str);
    } while (*str++);

    return nullptr;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strnchr(const CharT* str, CharT c, size_t n)
{
    while (n-- > 0) {
        if (*str == c)
            return const_cast<CharT*>(str);
        if (*str == '\0')
            return nullptr;

        ++str;
    }

    return nullptr;
}

template <typename CharT, typename = std::enable_if_t<is_char_type<CharT>::value>>
IOCOD_EXPORT constexpr CharT* Strrchr(const CharT* str, CharT c)
{
    const CharT* found = nullptr;
    CharT current;

    while ((current = *str++) != 0) {
        if (current == c)
            found = (str - 1);
    }

    if (found)
        return const_cast<CharT*>(found);

    return c ? nullptr : const_cast<CharT*>(str - 1);
}

} // namespace iocod

#endif // CORE_STRING_H
