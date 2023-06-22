// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRING_H
#define CORE_STRING_H

#include <Core/Assert.h>
#include <Core/Platform.h>

namespace iocod {

class FString {
public:
    static inline constexpr std::size_t kBaseSize = 20;
    static inline constexpr std::size_t kGranularity = 32;

    FString() { Construct(); }
    ~FString() { Free(); }

    explicit FString(const char* text);

    [[nodiscard]] std::size_t Size() const { return sizeof(*this) + Allocated(); }
    [[nodiscard]] std::size_t Length() const { return m_length; }

    [[nodiscard]] std::size_t Allocated() const
    {
        if (m_data != m_base)
            return GetAllocated();

        return 0;
    }

    void Clear();
    [[nodiscard]] bool IsEmpty() const;

    [[nodiscard]] const char* CStr() const { return m_data; }

    [[nodiscard]] explicit operator const char*() { return CStr(); }
    [[nodiscard]] explicit operator const char*() const { return CStr(); }

    [[nodiscard]] char operator[](const std::size_t index)
    {
        Assert(index <= m_length);
        return m_data[index];
    }

    [[nodiscard]] char operator[](const std::size_t index) const
    {
        Assert(index <= m_length);
        return m_data[index];
    }

    void Reallocate(std::size_t amount, bool keepOld);
    void Free();

protected:
    std::size_t m_length{0};
    char* m_data{nullptr};
    char m_base[kBaseSize]{};

    void EnsureAllocated(std::size_t amount, bool keepOld = true);

private:
    void Construct();

    std::size_t m_allocated{0};

    [[nodiscard]] std::size_t GetAllocated() const { return m_allocated; }
    void SetAllocated(const std::size_t amount) { m_allocated = amount; }
};

//
// string.h library replacements for all char types
//

template <typename T>
struct IsCharType : std::false_type {};

template <>
struct IsCharType<char> : std::true_type {};

template <>
struct IsCharType<wchar_t> : std::true_type {};

template <>
struct IsCharType<char8_t> : std::true_type {};

template <>
struct IsCharType<char16_t> : std::true_type {};

template <>
struct IsCharType<char32_t> : std::true_type {};

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strcpy(CharT* dest, const CharT* src)
{
    const CharT* s = src;
    CharT* d = dest;

    while ((*d++ = *s++) != 0) {}

    return dest;
}

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strncpy(CharT* dest, const CharT* src, size_t n)
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

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strcat(CharT* dest, const CharT* src)
{
    const CharT* s = src;
    CharT* d = dest;

    while (*d++) {}
    d--;
    while ((*d++ = *s++) != 0) {}

    return dest;
}

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strncat(CharT* dest, const CharT* src, size_t n)
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

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr size_t Strlen(const CharT* str)
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

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strend(const CharT* str)
{
    while (*str)
        ++str;

    return const_cast<CharT*>(str);
}

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strdup(const CharT* str)
{
    // TODO: custom memory allocators
    if (!str)
        return nullptr;

    const size_t len = Strlen(str);
    char* const dup = new char[len + 1];

    Strcpy(dup, str);
    return dup;
}

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strdel(const CharT* str)
{
    // TODO: custom memory allocators
    delete[] str;
}

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strchr(const CharT* str, CharT c)
{
    do {
        if (*str == c)
            return const_cast<CharT*>(str);
    } while (*str++);

    return nullptr;
}

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strnchr(const CharT* str, CharT c, size_t n)
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

template <typename CharT, typename = std::enable_if_t<IsCharType<CharT>::value>>
constexpr CharT* Strrchr(const CharT* str, CharT c)
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
