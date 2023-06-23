// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRING_H
#define CORE_STRING_H

#include <Core/Assert.h>
#include <Core/Platform.h>

namespace iocod {

/**
 * \brief String class which internally uses UTF-8 strings, convertible to
 * wide char strings. Has similar STL capabilities as std::string.
 */
class IOCOD_EXPORT FString {
public:
    static inline constexpr std::size_t npos = 18446744073709551615ull;
    static inline constexpr std::size_t kBaseSize = 20ull;
    static inline constexpr std::size_t kGranularity = 32ull;

    // missing: STL container requirements
    // allocator_type
    // iterator
    // const_iterator
    // reverse_iterator
    // const_reverse_iterator
    using ValueType = char;
    using SizeType = std::size_t;
    using DifferenceType = std::ptrdiff_t;
    using Reference = ValueType&;
    using ConstReference = const ValueType&;
    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;

    constexpr FString() noexcept = default;

    constexpr FString(const FString& other, SizeType pos);
    constexpr FString(FString&& other, SizeType pos);
    constexpr FString(const FString& other, SizeType pos, SizeType count);
    constexpr FString(FString&& other, SizeType pos, SizeType count);
    constexpr FString(const FString& other);
    constexpr FString(FString&& other) noexcept;

    /**
     * \brief Construct a string of count characters, each initialized to ch.
     * \param[in] count number of characters
     * \param[in] ch character to initialize each character to
     */
    constexpr FString(SizeType count, char ch);

    /**
     * \brief Construct a string from a null-terminated string, copying at most
     * count characters. If count exceeds the length of the string, will only
     * copy up to the null terminator.
     * \param[in] s string
     * \param[in] count number of characters to copy
     */
    constexpr FString(const char* s, SizeType count);

    /**
     * \brief Construct a string from a null-terminated string.
     * \param[in] s string
     */
    explicit constexpr FString(const char* s);

    /**
     * \brief Construct a string from a substring of another string.
     * \param[in] s string 
     * \param[in] pos starting position
     * \param[in] count number of characters to copy
     */
    constexpr FString(const char* s, SizeType pos, SizeType count);

    /**
     * \brief Construct a string with a boolean value of '0' or '1'.
     * \param[in] b boolean value
     */
    explicit constexpr FString(bool b);

    /**
     * \brief Construct a string with a single character.
     * \param c character
     */
    explicit constexpr FString(char c);

    // TODO: maybe consider a better way to do this? constexpr?
    explicit FString(i32 i);
    explicit FString(i64 i);
    explicit FString(u32 u);
    explicit FString(u64 u);
    explicit FString(f32 f);
    explicit FString(f64 f);
    // end TODO

    ~FString();

    constexpr FString& Assign(SizeType count, char ch);
    constexpr FString& Assign(const FString& str);
    constexpr FString& Assign(const FString& str, SizeType pos, SizeType count = npos);
    constexpr FString& Assign(FString&& str) noexcept;
    constexpr FString& Assign(const char* s, SizeType count);
    constexpr FString& Assign(const char* s);

    constexpr Reference At(SizeType pos);
    [[nodiscard]] constexpr ConstReference At(SizeType pos) const;
    constexpr Reference operator[](SizeType pos);
    constexpr ConstReference operator[](SizeType pos) const;

    [[nodiscard]] constexpr Reference Front();
    [[nodiscard]] constexpr ConstReference Front() const;
    [[nodiscard]] constexpr Reference Back();
    [[nodiscard]] constexpr ConstReference Back() const;
    [[nodiscard]] constexpr ConstPointer Data() const noexcept { return m_data; }
    [[nodiscard]] constexpr Pointer Data() noexcept { return m_data; }
    [[nodiscard]] constexpr ConstPointer c_str() const noexcept { return m_data; }

    [[nodiscard]] constexpr bool Empty() const noexcept { return m_length == 0; }
    [[nodiscard]] constexpr SizeType Size() const noexcept;
    [[nodiscard]] constexpr SizeType Length() const noexcept { return m_length; }
    [[nodiscard]] constexpr SizeType MaxSize() const noexcept;
    constexpr void Reserve(SizeType newCapacity);
    [[nodiscard]] constexpr SizeType Capacity() const noexcept { return m_allocated; }
    constexpr void ShrinkToFit();

    constexpr void Clear() noexcept;

    // missing STL string
    // constexpr FString& Insert(SizeType index, SizeType count, char ch);
    // constexpr FString& Insert(SizeType index, const char* s);
    // constexpr FString& Insert(SizeType index, const char* s, SizeType count);
    // constexpr FString& Insert(SizeType index, const FString& str);
    // constexpr FString& Insert(SizeType index, const FString& str, SizeType indexStr, SizeType count = npos);
    // constexpr Iterator Insert(ConstIterator pos, char ch);
    // constexpr Iterator Insert(ConstIterator pos, SizeType count, char ch);

    constexpr FString& Erase(SizeType index = 0, SizeType count = npos);
    // constexpr Iterator Erase(ConstIterator pos);
    // constexpr Iterator Erase(ConstIterator first, ConstIterator last);

    /**
     * \brief Fill entire string with character, up to count or length (which ever is smaller)
     * \param[in] c character to fill string with
     * \param[in] count number of characters to fill string with
     */
    constexpr void Fill(char c, SizeType count = npos) const noexcept;

    constexpr void ToLower();
    constexpr void ToUpper();

    /**
     * \brief Append a single character to the end of the string.
     * \param[in] c character to append
     */
    constexpr void Append(char c);

    constexpr void Append(const FString& str);

    /**
     * \brief Append a null-terminated string to the end of the string.
     * \param[in] str string to append
     */
    constexpr void Append(const char* str);

    /**
     * \brief Append a substring of another string. If length < str length, will only copy the
     * actual string length.
     * \param[in] str string to append
     * \param[in] length number of characters to copy
     */
    constexpr void Append(const char* str, SizeType length);

    constexpr FString& operator=(const FString& str);
    constexpr FString& operator=(FString&& str) noexcept;
    constexpr FString& operator=(const char* s);
    constexpr FString& operator=(char c);

    constexpr FString& operator+=(const FString& str);

    constexpr FString& operator+=(const char c)
    {
        Append(c);
        return *this;
    }

    constexpr FString& operator+=(const char* s)
    {
        Append(s);
        return *this;
    }

    constexpr FString& operator+=(i64 i);
    constexpr FString& operator+=(u64 u);
    constexpr FString& operator+=(f32 f);

    constexpr FString& operator+=(const bool b)
    {
        Append(b ? "true" : "false");
        return *this;
    }

    static FString FromWideString(const wchar_t* s);
    static void ToWideString(const FString& str, wchar_t* dest, SizeType destSize);

    //
    // static methods
    //
    static SizeType Length(const char* s);
    static char ToLower(char c);
    static char* ToLower(char* s);
    static char ToUpper(char c);
    static char* ToUpper(char* s);

protected:
    SizeType m_length{0};
    SizeType m_allocated{kBaseSize};
    char* m_data{m_baseBuffer};
    char m_baseBuffer[kBaseSize]{};

private:
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
