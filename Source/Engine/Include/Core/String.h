// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRING_H
#define CORE_STRING_H

#include <stdexcept>
#include <Core/Assert.h>

namespace iocod {

template <typename T>
struct StringIterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    StringIterator() noexcept = default;

    constexpr explicit StringIterator(const pointer ptr) noexcept :
        ptr(ptr) {}

    constexpr pointer operator->() const noexcept { return ptr; }
    constexpr reference operator*() const noexcept { return *ptr; }
    constexpr explicit operator pointer() const noexcept { return ptr; }

    constexpr StringIterator& operator++() noexcept
    {
        ++ptr;
        return *this;
    }

    constexpr StringIterator operator++(int) noexcept
    {
        const StringIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    constexpr StringIterator& operator--() noexcept
    {
        --ptr;
        return *this;
    }

    constexpr StringIterator operator--(int) noexcept
    {
        const StringIterator tmp = *this;
        --(*this);
        return tmp;
    }

    constexpr bool operator==(const StringIterator& rhs) const noexcept { return ptr == rhs.ptr; }
    constexpr bool operator!=(const StringIterator& rhs) const noexcept { return ptr != rhs.ptr; }

    pointer ptr;
};

/**
 * \brief String class which internally uses UTF-8 strings, convertible to
 * wide char strings. Has similar STL capabilities as std::string.
 */
class IOCOD_API String {
public:
    static inline constexpr std::size_t npos = 18446744073709551615ull;

    // keep minimum size of 40 bytes, and allocate in multiples of 64 bytes
    // most cpu cache lines are 64 bytes, so this should be a good size
    // 8  std::size_t m_length
    // 8  std::size_t m_allocated
    // 8  char* m_data
    // 40 char m_baseBuffer[]
    // ---
    // 64
    // TODO: consider SSO stuff that STL does
    // may or may not make a difference, but it's worth looking into
    static inline constexpr std::size_t kBaseSize = 40ull; // minimum size of the base buffer
    static inline constexpr std::size_t kGranularity = 64ull; // size to allocate in multiples of

    using value_type = char;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = StringIterator<value_type>;
    using const_iterator = StringIterator<const value_type>;

    //
    // constructors
    //

    constexpr String() noexcept = default;
    constexpr String(const String& other);
    constexpr String(String&& other) noexcept;
    constexpr String& operator=(const String& other);
    constexpr String& operator=(String&& other) noexcept;

    constexpr String(size_type count, char ch);

    constexpr /* implicit */ String(const char* s, size_type count = npos);
    constexpr String(const char* s, size_type pos, size_type count);

    // TODO: constexpr
    explicit String(const wchar_t* s, size_type count = npos);

    explicit constexpr String(bool b);
    explicit constexpr String(char c);

    // TODO: maybe consider a better way to do this? constexpr?
    explicit String(i32 i);
    explicit String(i64 i);
    explicit String(u32 u);
    explicit String(u64 u);
    explicit String(f32 f);
    explicit String(f64 f);
    // end TODO

    explicit constexpr String(const std::string& str);
    explicit constexpr String(std::string&& str) noexcept;
    constexpr String& operator=(const std::string& other);
    constexpr String& operator=(std::string&& other) noexcept;

    ~String();

    //
    // methods
    //

    // TODO: Substring
    // TODO: Find

    [[nodiscard]] constexpr reference At(size_type pos);
    [[nodiscard]] constexpr const_reference At(size_type pos) const;
    constexpr reference operator[](size_type pos);
    constexpr const_reference operator[](size_type pos) const;

    [[nodiscard]] constexpr reference Front() { return *m_data; }
    [[nodiscard]] constexpr const_reference Front() const { return *m_data; }
    [[nodiscard]] constexpr reference Back() { return *(m_data + m_length - 1); }
    [[nodiscard]] constexpr const_reference Back() const { return *(m_data + m_length - 1); }

    constexpr void Clear() noexcept;

    // missing STL string methods
    // constexpr FString& Insert(SizeType index, SizeType count, char ch);
    // constexpr FString& Insert(SizeType index, const char* s);
    // constexpr FString& Insert(SizeType index, const char* s, SizeType count);
    // constexpr FString& Insert(SizeType index, const FString& str);
    // constexpr FString& Insert(SizeType index, const FString& str, SizeType indexStr, SizeType count = npos);
    // constexpr Iterator Insert(ConstIterator pos, char ch);
    // constexpr Iterator Insert(ConstIterator pos, SizeType count, char ch);
    // constexpr FString& Erase(SizeType index = 0, SizeType count = npos);
    // constexpr Iterator Erase(ConstIterator pos);
    // constexpr Iterator Erase(ConstIterator first, ConstIterator last);

    constexpr void Fill(char c, size_type count = npos) const noexcept;
    constexpr void Append(char c);
    constexpr void Append(const String& str);
    constexpr void Append(const char* str);
    constexpr void Append(const char* str, size_type length);
    constexpr void Copy(const char* dest) const noexcept;
    constexpr void Copy(const char* dest, size_type count) const noexcept;
    constexpr bool Compare(const char* str) const noexcept;
    constexpr bool Compare(const char* str, size_type count) const noexcept;
    constexpr bool CompareIgnoreCase(const char* str) const noexcept;
    constexpr bool CompareIgnoreCase(const char* str, size_type count) const noexcept;

    //
    // string utilities (chainable)
    //

    constexpr String& Capitalize();
    constexpr String& Clean();
    constexpr String& Replace(const String& from, const String& to);

    constexpr String& ToLower() noexcept;
    constexpr String& ToUpper() noexcept;

    // constexpr String& Trim()
    // {
    //     return TrimLeft().TrimRight();
    // }
    // 
    // constexpr String& TrimLeft();
    // constexpr String& TrimRight();

    constexpr String& Escape();
    constexpr String& Unescape();

    constexpr String& StripColors();
    constexpr String& StripExtension();
    constexpr String& StripFileName();
    constexpr String& StripPath();

    constexpr String& ConvertSlashes();
    constexpr String& SetExtension(const String& ext);
    constexpr String& DefaultExtension(const String& ext);
    constexpr String& DefaultPath(const String& ext);

    //
    // misc utilities
    //

    [[nodiscard]] String Left(const size_type count) const noexcept
    {
        return Middle(0, count);
    }

    [[nodiscard]] String Right(const size_type count) const noexcept
    {
        if (count >= m_length)
            return *this;

        return Middle(Length() - count, count);
    }

    [[nodiscard]] String Middle(const size_type pos, size_type count) const noexcept
    {
        const std::size_t length = Length();
        if (length == 0 || count == 0 || pos >= length)
            return {};

        if (pos + count >= length)
            count = length - pos;

        return String{m_data + pos, count};
    }

    [[nodiscard]] String Extension()
    {
        std::size_t pos = Length() - 1;
        while ((pos > 0) && ((*this)[pos - 1] != '.'))
            --pos;

        if (!pos)
            return {};

        return Right(Length() - pos);
    }

    [[nodiscard]] String FileName()
    {
        std::size_t pos = Length() - 1;

        while ((pos > 0) && ((*this)[pos - 1] != '/') && ((*this)[pos - 1] != '\\'))
            --pos;

        return Right(Length() - pos);
    }

    [[nodiscard]] String Path()
    {
        std::size_t pos = Length() - 1;

        while ((pos > 0) && ((*this)[pos - 1] != '/') && ((*this)[pos - 1] != '\\'))
            --pos;

        return Left(pos);
    }

    constexpr bool HasExtension(const String& ext) const noexcept;

    constexpr bool Contains(const String& str) const noexcept;
    constexpr bool Contains(const char* str) const noexcept;
    constexpr bool EndsWith(const String& str) const noexcept;
    constexpr bool EndsWith(const char* str) const noexcept;

    [[nodiscard]] wchar_t* ToWideString() const noexcept;
    void ToWideString(wchar_t* dest, size_type size) const noexcept;
    [[nodiscard]] size_type WideStringSize() const noexcept;

    //
    // operators
    //

    constexpr String& operator=(const char* s);

    constexpr String& operator+=(const String& str);
    constexpr String& operator+=(char c);
    constexpr String& operator+=(const char* s);
    String& operator+=(i32 i);
    String& operator+=(i64 i);
    String& operator+=(u32 i);
    String& operator+=(u64 u);
    String& operator+=(f32 f);
    String& operator+=(f64 f);
    constexpr String& operator+=(bool b);

    constexpr String& operator+=(const std::string& str);

    constexpr String& operator/=(const String& str);
    constexpr String& operator/=(const char* s);

    friend String operator+(const String& lhs, const String& rhs);
    friend String operator+(const String& lhs, char c);
    friend String operator+(const String& lhs, const char* s);
    friend String operator+(const String& lhs, i32 i);
    friend String operator+(const String& lhs, i64 i);
    friend String operator+(const String& lhs, u32 i);
    friend String operator+(const String& lhs, u64 u);
    friend String operator+(const String& lhs, f32 f);
    friend String operator+(const String& lhs, f64 f);
    friend String operator+(const String& lhs, bool b);

    friend String operator+(const String& lhs, const std::string& rhs);

    friend bool operator==(const String& lhs, const String& rhs);
    friend bool operator==(const String& lhs, const char* rhs);
    friend bool operator==(const char* lhs, const String& rhs);
    friend bool operator!=(const String& lhs, const String& rhs);
    friend bool operator!=(const String& lhs, const char* rhs);
    friend bool operator!=(const char* lhs, const String& rhs);

    friend String operator/(const String& lhs, const String& rhs);
    friend String operator/(const String& lhs, const char* rhs);

    //
    // STL compatibility (range-for)
    //

    iterator begin() noexcept
    {
        return iterator(&m_data[0]);
    }

    iterator end() noexcept
    {
        return iterator(&m_data[m_length]);
    }

    [[nodiscard]] iterator begin() const noexcept
    {
        return iterator(&m_data[0]);
    }

    [[nodiscard]] iterator end() const noexcept
    {
        return iterator(&m_data[m_length]);
    }

    [[nodiscard]] const_iterator cbegin() const noexcept
    {
        return const_iterator(&m_data[0]);
    }

    [[nodiscard]] const_iterator cend() const noexcept
    {
        return const_iterator(&m_data[m_length]);
    }

    //
    // static methods
    //

    static constexpr size_type Length(const char* s) noexcept;
    static constexpr char ToLower(char c) noexcept;
    static constexpr char* ToLower(char* s) noexcept;
    static constexpr char ToUpper(char c) noexcept;
    static constexpr char* ToUpper(char* s) noexcept;
    static constexpr char* Copy(char* dest, const char* src) noexcept;
    static constexpr char* Copy(char* dest, const char* src, size_type count) noexcept;
    static constexpr int Compare(const char* lhs, const char* rhs) noexcept;
    static constexpr int Compare(const char* lhs, const char* rhs, size_type count) noexcept;
    static constexpr int CompareIgnoreCase(const char* lhs, const char* rhs) noexcept;
    static constexpr int CompareIgnoreCase(const char* lhs, const char* rhs,
                                           size_type count) noexcept;
    static String FromWideString(const wchar_t* s);
    static void ToWideString(const char* str, wchar_t* dest, size_type destSize);

    //
    // simple inlined functions
    //

    [[nodiscard]] constexpr const_pointer Data() const noexcept
    {
        return m_data;
    }

    [[nodiscard]] constexpr pointer Data() noexcept
    {
        return m_data;
    }

    [[nodiscard]] constexpr const_pointer c_str() const noexcept
    {
        return m_data;
    }

    [[nodiscard]] constexpr bool Empty() const noexcept
    {
        return m_length == 0;
    }

    [[nodiscard]] constexpr size_type Size() const noexcept
    {
        return sizeof(*this) + Allocated();
    }

    [[nodiscard]] constexpr size_type Length() const noexcept
    {
        return m_length;
    }

    constexpr void Reserve(size_type newCapacity);

    [[nodiscard]] constexpr size_type Capacity() const noexcept
    {
        return m_allocated;
    }

    [[nodiscard]] constexpr size_type Allocated() const noexcept
    {
        return m_data != m_baseBuffer ? m_allocated : 0;
    }

protected:
    size_type m_length{0};
    size_type m_allocated{kBaseSize};
    char* m_data{m_baseBuffer};
    char m_baseBuffer[kBaseSize]{};
};

// --------------------------------
// String::String
// --------------------------------
constexpr String::String(const String& other)
{
    const std::size_t length = other.m_length;
    if (length > m_allocated)
        Reserve(length + 1);

    Copy(m_data, other.m_data);
    m_data[length] = 0;
    m_length = length;
}

constexpr String::String(String&& other) noexcept
{
    *this = std::move(other);
}

constexpr String::String(const size_type count, const char ch)
{
    if (count > m_allocated)
        Reserve(count);

    for (size_type i = 0; i < count; ++i)
        m_data[i] = ch;

    m_data[count] = 0;
    m_length = count;
}

constexpr String::String(const char* s, size_type count)
{
    Assert(s);

    if (const std::size_t len = Length(s); count > len)
        count = len;

    if (count > m_allocated)
        Reserve(count);

    for (size_type i = 0; i < count; ++i)
        m_data[i] = s[i];

    m_data[count] = 0;
    m_length = count;
}

constexpr String::String(const char* s, const size_type pos, const size_type count)
{
    Assert(s);

    const std::size_t len = Length(s);
    std::size_t start = pos;
    std::size_t end = pos + count;

    if (end > len)
        end = len;

    if (start > len)
        start = len;

    const std::size_t newSize = end - start;
    if (newSize > m_allocated)
        Reserve(newSize);

    for (size_type i = 0; i < newSize; ++i)
        m_data[i] = s[start + i];

    m_data[newSize] = 0;
    m_length = newSize;
}

constexpr String::String(const bool b)
{
    m_data[0] = b ? '1' : '0';
    m_data[1] = 0;
    m_length = 1;
}

constexpr String::String(const char c)
{
    m_data[0] = c;
    m_data[1] = 0;
    m_length = 1;
}

constexpr String::String(const std::string& str)
{
    const std::size_t length = str.length();
    if (length > m_allocated)
        Reserve(length);

    Copy(m_data, str.c_str());
    m_length = length;
}

constexpr String::String(std::string&& str) noexcept
{
    *this = std::move(str);
}

// --------------------------------
// String::At
// --------------------------------
constexpr String::reference String::At(const size_type pos)
{
    if (pos >= m_length)
        throw std::out_of_range("out of range");

    return m_data[pos];
}

constexpr String::const_reference String::At(const size_type pos) const
{
    if (pos >= m_length)
        throw std::out_of_range("out of range");

    return m_data[pos];
}

// --------------------------------
// String::operator[]
// --------------------------------
constexpr String::reference String::operator[](const size_type pos)
{
    Assert(pos < m_length);
    return m_data[pos];
}

constexpr String::const_reference String::operator[](const size_type pos) const
{
    Assert(pos < m_length);
    return m_data[pos];
}

// --------------------------------
// String::Fill
// --------------------------------
constexpr void String::Fill(const char c, const size_type count) const noexcept
{
    std::size_t max = count;
    if (max == npos || count > m_length)
        max = m_length;

    for (std::size_t i = 0; i < max; i++)
        m_data[i] = c;

    m_data[max] = 0;
}

// --------------------------------
// String::ToLower
// --------------------------------
constexpr String& String::ToLower() noexcept
{
    for (std::size_t i = 0; i < m_length; i++)
        m_data[i] = ToLower(m_data[i]);

    return *this;
}

// --------------------------------
// String::ToUpper
// --------------------------------
constexpr String& String::ToUpper() noexcept
{
    for (std::size_t i = 0; i < m_length; i++)
        m_data[i] = ToUpper(m_data[i]);

    return *this;
}

// --------------------------------
// String::Append
// --------------------------------
constexpr void String::Append(const char c)
{
    const std::size_t newLength = m_length + 1;
    if (newLength >= m_allocated)
        Reserve(newLength + 1);

    m_data[m_length] = c;

    m_length = newLength;
    m_data[m_length] = 0;
}

constexpr void String::Append(const String& str)
{
    const std::size_t newLength = m_length + str.m_length;
    if (newLength >= m_allocated)
        Reserve(newLength + 1);

    for (std::size_t i = 0; i < str.m_length; i++)
        m_data[m_length + i] = str.m_data[i];

    m_length = newLength;
    m_data[m_length] = 0;
}

constexpr void String::Append(const char* str)
{
    if (!str)
        return;

    const std::size_t newLength = m_length + Length(str);
    if (newLength >= m_allocated)
        Reserve(newLength + 1);

    for (std::size_t i = 0; i < Length(str); i++)
        m_data[m_length + i] = str[i];

    m_length = newLength;
    m_data[m_length] = 0;
}

constexpr void String::Append(const char* str, size_type length)
{
    if (!str || !length)
        return;

    if (const std::size_t len = Length(str); len < length)
        length = len;

    const std::size_t newLength = length + m_length;
    if (newLength >= m_allocated)
        Reserve(newLength + 1);

    for (std::size_t i = 0; i < length; i++)
        m_data[m_length + i] = str[i];

    m_length = newLength;
    m_data[m_length] = 0;
}

// --------------------------------
// String::Copy
// --------------------------------
constexpr void String::Copy(const char* dest) const noexcept
{
    Copy(m_data, dest);
}

constexpr void String::Copy(const char* dest, const size_type count) const noexcept
{
    Copy(m_data, dest, count);
}

// --------------------------------
// String::Compare / CompareIgnoreCase
// --------------------------------
constexpr bool String::Compare(const char* str) const noexcept
{
    return Compare(m_data, str) == 0;
}

constexpr bool String::Compare(const char* str, const size_type count) const noexcept
{
    return Compare(m_data, str, count) == 0;
}

constexpr bool String::CompareIgnoreCase(const char* str) const noexcept
{
    return CompareIgnoreCase(m_data, str) == 0;
}

constexpr bool String::CompareIgnoreCase(const char* str, const size_type count) const noexcept
{
    return CompareIgnoreCase(m_data, str, count) == 0;
}

// --------------------------------
// String::ToWideString
// --------------------------------
inline wchar_t* String::ToWideString() const noexcept
{
    static wchar_t s_buffer[1024]{};
    ToWideString(m_data, s_buffer, sizeof(s_buffer));
    return s_buffer;
}

inline void String::ToWideString(wchar_t* dest, const size_type size) const noexcept
{
    ToWideString(m_data, dest, size);
}

// --------------------------------
// String::operator=
// --------------------------------
constexpr String& String::operator=(const char* s)
{
    Clear();
    Append(s);
    return *this;
}

// --------------------------------
// String::operator+=
// --------------------------------
constexpr String& String::operator+=(const String& str)
{
    Append(str);
    return *this;
}

constexpr String& String::operator+=(const char c)
{
    Append(c);
    return *this;
}

constexpr String& String::operator+=(const char* s)
{
    Append(s);
    return *this;
}

// weird behavior from doom3:
// on FString(bool) constructor, converts bool to '1' or '0'
// but here it converts to 'true' or 'false'
// TODO: consolidate/fix this??
constexpr String& String::operator+=(const bool b)
{
    Append(b ? "true" : "false");
    return *this;
}

constexpr String& String::operator+=(const std::string& str)
{
    Append(str.c_str());
    return *this;
}

// --------------------------------
// String::operator/=
// --------------------------------
constexpr String& String::operator/=(const String& str)
{
    Append('/');
    Append(str);
    return *this;
}

constexpr String& String::operator/=(const char* s)
{
    Append('/');
    Append(s);
    return *this;
}

// --------------------------------
// operator+
// --------------------------------
inline String operator+(const String& lhs, const String& rhs)
{
    String result{lhs};
    result += rhs;
    return result;
}

inline String operator+(const String& lhs, const char c)
{
    String result{lhs};
    result += c;
    return result;
}

inline String operator+(const String& lhs, const char* s)
{
    String result{lhs};
    result += s;
    return result;
}

inline String operator+(const String& lhs, const i32 i)
{
    String result{lhs};
    result += i;
    return result;
}

inline String operator+(const String& lhs, const i64 i)
{
    String result{lhs};
    result += i;
    return result;
}

inline String operator+(const String& lhs, const u32 i)
{
    String result{lhs};
    result += i;
    return result;
}

inline String operator+(const String& lhs, const u64 u)
{
    String result{lhs};
    result += u;
    return result;
}

inline String operator+(const String& lhs, const f32 f)
{
    String result{lhs};
    result += f;
    return result;
}

inline String operator+(const String& lhs, const f64 f)
{
    String result{lhs};
    result += f;
    return result;
}

inline String operator+(const String& lhs, const bool b)
{
    String result{lhs};
    result += b;
    return result;
}

inline String operator+(const String& lhs, const std::string& rhs)
{
    String result{lhs};
    result += rhs.c_str();
    return result;
}

// --------------------------------
// operator==
// --------------------------------
inline bool operator==(const String& lhs, const String& rhs)
{
    return String::Compare(lhs.m_data, rhs.m_data) == 0;
}

inline bool operator==(const String& lhs, const char* rhs)
{
    Assert(rhs);
    return String::Compare(lhs.m_data, rhs) == 0;
}

inline bool operator==(const char* lhs, const String& rhs)
{
    Assert(lhs);
    return String::Compare(lhs, rhs.m_data) == 0;
}

// --------------------------------
// operator!=
// --------------------------------
inline bool operator!=(const String& lhs, const String& rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(const String& lhs, const char* rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(const char* lhs, const String& rhs)
{
    return !(lhs == rhs);
}

// --------------------------------
// operator/
// --------------------------------
inline String operator/(const String& lhs, const String& rhs)
{
    String result{lhs};
    result /= rhs;
    return result;
}

inline String operator/(const String& lhs, const char* rhs)
{
    String result{lhs};
    result /= rhs;
    return result;
}

//
// inlined static functions
//
constexpr String::size_type String::Length(const char* s) noexcept
{
    #if IOCOD_HAS_BUILTIN(__builtin_strlen)
    return __builtin_strlen(str);
    #else
    std::size_t len = 0;

    do {
        ++len;
    } while (*s++);

    return len - 1;
    #endif
}

constexpr char String::ToLower(const char c) noexcept
{
    if (c <= 'Z' && c >= 'A')
        return c + ('a' - 'A');

    return c;
}

constexpr char* String::ToLower(char* s) noexcept
{
    char* p = s;

    while (*p) {
        const char c = *p;
        *p++ = ToLower(c);
    }

    return p;
}

constexpr char String::ToUpper(const char c) noexcept
{
    if (c >= 'a' && c <= 'z')
        return c - ('a' - 'A');

    return c;
}

constexpr char* String::ToUpper(char* s) noexcept
{
    char* p = s;

    while (*p) {
        const char c = *p;
        *p++ = ToUpper(c);
    }

    return p;
}

constexpr char* String::Copy(char* dest, const char* src) noexcept
{
    const char* s = src;
    char* d = dest;

    while ((*d++ = *s++) != 0) {}

    return dest;
}

constexpr char* String::Copy(char* dest, const char* src, size_type count) noexcept
{
    const char* s = src;
    char* d = dest;

    count++;
    while (--count) {
        if ((*d++ = *s++) == 0) {
            while (--count)
                *d++ = 0;

            break;
        }
    }

    return dest;
}

constexpr int String::Compare(const char* lhs, const char* rhs) noexcept
{
    char c1;
    char c2;

    while ((c1 = *lhs++) == (c2 = *rhs++)) {
        if (c1 == 0)
            return 0;
    }

    return c1 - c2;
}

constexpr int String::Compare(const char* lhs, const char* rhs, size_type count) noexcept
{
    char c1;
    char c2;

    ++count;
    while (--count) {
        if ((c1 = *lhs++) != (c2 = *rhs++))
            return c1 - c2;
        if (c1 == 0)
            return 0;
    }

    return 0;
}

constexpr int String::CompareIgnoreCase(const char* lhs, const char* rhs) noexcept
{
    char c1;
    char c2;

    while ((c1 = ToLower(*lhs++)) == (c2 = ToLower(*rhs++))) {
        if (c1 == 0)
            return 0;
    }

    return c1 - c2;
}

constexpr int String::CompareIgnoreCase(const char* lhs, const char* rhs, size_type count) noexcept
{
    char c1;
    char c2;

    ++count;
    while (--count) {
        if ((c1 = ToLower(*lhs++)) != (c2 = ToLower(*rhs++)))
            return c1 - c2;
        if (c1 == 0)
            return 0;
    }

    return 0;
}

inline String String::FromWideString(const wchar_t* s)
{
    Assert(s);

    return String{s};
}

//
// C <string.h> replacements for all char types
//

template <typename T>
struct IsCharType : std::false_type {};

template <typename T>
inline constexpr bool IsCharTypeV = IsCharType<T>::value;

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

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
constexpr CharT* Strcpy(CharT* dest, const CharT* src)
{
    const CharT* s = src;
    CharT* d = dest;

    while ((*d++ = *s++) != 0) {}

    return dest;
}

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
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

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
constexpr CharT* Strcat(CharT* dest, const CharT* src)
{
    const CharT* s = src;
    CharT* d = dest;

    while (*d++) {}
    --d;
    while ((*d++ = *s++) != 0) {}

    return dest;
}

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
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

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
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

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
constexpr CharT* Strend(const CharT* str)
{
    while (*str)
        ++str;

    return const_cast<CharT*>(str);
}

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
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

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
constexpr void Strdel(const CharT* str)
{
    // TODO: custom memory allocators
    delete[] str;
}

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
constexpr CharT* Strchr(const CharT* str, CharT c)
{
    do {
        if (*str == c)
            return const_cast<CharT*>(str);
    } while (*str++);

    return nullptr;
}

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
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

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
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

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
constexpr int Strcmp(const CharT* lhs, const CharT* rhs)
{
    CharT c1;
    CharT c2;

    while ((c1 = *lhs++) == (c2 = *rhs++)) {
        if (c1 == 0)
            return 0;
    }

    return (c1 > c2) ? 1 : -1;
}

template <typename CharT, typename = std::enable_if_t<IsCharTypeV<CharT>>>
constexpr int Strncmp(const CharT* lhs, const CharT* rhs, std::size_t n)
{
    CharT c1;
    CharT c2;

    while (n-- > 0) {
        if ((c1 = *lhs++) != (c2 = *rhs++))
            return (c1 > c2) ? 1 : -1;
        if (c1 == 0)
            return 0;
    }

    return 0;
}

// TODO: stricmp / strnicmp for utf16/32

/**
 * \brief Light-weight wrapper around a C-style strings.
 */
class IOCOD_API CString {
public:
    constexpr /* implicit */ CString(const char* str) : m_str(str) {}

    bool operator=(const char* other) = delete;

    const char* c_str() const { return m_str; }
    explicit operator const char*() const { return m_str; }

    friend bool operator==(const CString& lhs, const CString& rhs) { return Strcmp(lhs.m_str, rhs.m_str) == 0; }
    friend bool operator!=(const CString& lhs, const CString& rhs) { return !(lhs == rhs); }
    friend std::strong_ordering operator<=>(const CString& lhs, const CString& rhs) { return Strcmp(lhs.m_str, rhs.m_str) <=> 0; }

private:
    const char* m_str;
};

class IOCOD_API StringLiteral {
public:
    template <std::size_t N>
    explicit(false) consteval StringLiteral(const char (&str)[N]) : m_size(N - 1), m_str(str)
    {
        Assert(*std::next(m_str, N - 1) == '\0');
    }

    constexpr StringLiteral() noexcept : m_size(0), m_str("") {}

    [[nodiscard]] constexpr std::size_t Size() const { return m_size; }
    [[nodiscard]] constexpr const char* c_str() const { return m_str; }
    explicit(false) constexpr operator const char*() const { return m_str; }

private:
    std::size_t m_size;
    const char* m_str;
};

} // namespace iocod

//
// hash specialization
//
#include <string_view>

template <>
struct std::hash<iocod::String> {
    std::size_t operator()(const iocod::String& s) const noexcept
    {
        return std::hash<std::string_view>{}(std::string_view{s.c_str(), s.Length()});
    }
};

//
// fmtlib specialization
//
#include <fmt/format.h>

template <>
struct fmt::formatter<iocod::String> : formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const iocod::String& s, FormatContext& ctx) const
    {
        return formatter<std::string_view>::format(std::string_view{s.c_str(), s.Length()}, ctx);
    }
};

#endif // CORE_STRING_H
