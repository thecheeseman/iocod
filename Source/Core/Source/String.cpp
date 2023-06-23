// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdexcept>
#include <Core/Memory.h>
#include <Core/String.h>

namespace iocod {

// --------------------------------
// String::Reserve
// --------------------------------
constexpr void FString::Reserve(const SizeType newCapacity)
{
    std::size_t newSize = newCapacity;
    if (const std::size_t mod = newCapacity % kGranularity; mod)
        newSize = newCapacity + kGranularity - mod;

    m_allocated = newSize;

    const auto newBuffer = static_cast<char*>(Memory::Alloc(newSize));
    m_data[m_length] = 0;
    Strcpy(newBuffer, m_data);

    if (m_data && m_data != m_baseBuffer)
        Memory::Free(m_data);

    m_data = newBuffer;
}

// --------------------------------
// String::String
// --------------------------------
constexpr FString::FString(const SizeType count, const char ch)
{
    if (count > m_allocated)
        Reserve(count);

    for (SizeType i = 0; i < count; ++i)
        m_data[i] = ch;

    m_data[count] = 0;
    m_length = count;
}

constexpr FString::FString(const char* s, SizeType count)
{
    if (const std::size_t len = Strlen(s); count > len)
        count = len;

    if (count > m_allocated)
        Reserve(count);

    for (SizeType i = 0; i < count; ++i)
        m_data[i] = s[i];

    m_data[count] = 0;
    m_length = count;
}

constexpr FString::FString(const char* s)
{
    const std::size_t count = Strlen(s);

    if (count > m_allocated)
        Reserve(count);

    for (SizeType i = 0; i < count; ++i)
        m_data[i] = s[i];

    m_data[count] = 0;
    m_length = count;
}

constexpr FString::FString(const char* s, const SizeType pos, const SizeType count)
{
    const std::size_t len = Strlen(s);
    std::size_t start = pos;
    std::size_t end = pos + count;

    if (end > len)
        end = len;

    if (start > len)
        start = len;

    const std::size_t newSize = end - start;
    if (newSize > m_allocated)
        Reserve(newSize);

    for (SizeType i = 0; i < newSize; ++i)
        m_data[i] = s[start + i];

    m_data[newSize] = 0;
    m_length = newSize;
}

constexpr FString::FString(const bool b)
{
    m_data[0] = b ? '1' : '0';
    m_data[1] = 0;
    m_length = 1;
}

constexpr FString::FString(const char c)
{
    m_data[0] = c;
    m_data[1] = 0;
    m_length = 1;
}

FString::FString(const i32 i)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%d", i);

    if (len > m_allocated)
        Reserve(len);

    Strcpy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

FString::FString(const i64 i)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%lld", i);

    if (len > m_allocated)
        Reserve(len);

    Strcpy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

FString::FString(const u32 u)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%u", u);

    if (len > m_allocated)
        Reserve(len);

    Strcpy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

FString::FString(const u64 u)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%llu", u);

    if (len > m_allocated)
        Reserve(len);

    Strcpy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

FString::FString(const f32 f)
{
    char buffer[64]{};

    std::size_t len = sprintf_s(buffer, "%f", f);

    while (len > 0 && buffer[len - 1] == '0')
        buffer[--len] = 0;

    while (len > 0 && buffer[len - 1] == '.')
        buffer[--len] = 0;

    if (len > m_allocated)
        Reserve(len);

    Strcpy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

FString::FString(const f64 f)
{
    char buffer[64]{};

    std::size_t len = sprintf_s(buffer, "%f", f);

    while (len > 0 && buffer[len - 1] == '0')
        buffer[--len] = 0;

    while (len > 0 && buffer[len - 1] == '.')
        buffer[--len] = 0;

    if (len > m_allocated)
        Reserve(len);

    Strcpy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

// --------------------------------
// String::~String
// --------------------------------
FString::~FString()
{
    if (m_data && m_data != m_baseBuffer)
        Memory::Free(m_data);
}

// --------------------------------
// String::At
// --------------------------------
constexpr FString::Reference FString::At(const SizeType pos)
{
    if (pos >= m_length)
        throw std::out_of_range("out of range");

    return m_data[pos];
}

constexpr FString::ConstReference FString::At(const SizeType pos) const
{
    if (pos >= m_length)
        throw std::out_of_range("out of range");

    return m_data[pos];
}

// --------------------------------
// String::operator[]
// --------------------------------
constexpr FString::Reference FString::operator[](const SizeType pos)
{
    Assert(pos < m_length);
    return m_data[pos];
}

constexpr FString::ConstReference FString::operator[](const SizeType pos) const
{
    Assert(pos < m_length);
    return m_data[pos];
}

// --------------------------------
// String::Clear
// --------------------------------
constexpr void FString::Clear() noexcept
{
    if (m_data && m_data != m_baseBuffer) {
        Memory::Free(m_data);
        m_data = m_baseBuffer;
    }

    m_data[0] = 0;
    m_length = 0;
}

// --------------------------------
// String::Fill
// --------------------------------
constexpr void FString::Fill(const char c, const SizeType count) const noexcept
{
    std::size_t max = count;
    if (max == npos || count > m_length)
        max = m_length;

    for (std::size_t i = 0; i < max; i++)
        m_data[i] = c;

    m_data[max] = 0;
}

constexpr void FString::Append(const char c)
{
    if (m_length + 1 >= m_allocated)
        Reserve(m_length + 1);

    m_data[m_length++] = c;
    m_data[m_length] = 0;
}

constexpr void FString::Append(const FString& str) {}

constexpr void FString::Append(const char* str)
{
    if (!str)
        return;

    const std::size_t newLength = m_length + Strlen(str);
    if (newLength >= m_allocated)
        Reserve(newLength + 1);

    for (std::size_t i = 0; i < newLength; i++)
        m_data[m_length + i] = str[i];

    m_length = newLength;
    m_data[m_length] = 0;
}

constexpr void FString::Append(const char* str, SizeType length)
{
    if (!str || !length)
        return;

    if (const std::size_t len = Strlen(str); len < length)
        length = len;

    const std::size_t newLength = length + m_length;
    if (newLength >= m_allocated)
        Reserve(newLength + 1);

    for (std::size_t i = 0; i < length; i++)
        m_data[m_length + i] = str[i];

    m_length = newLength;
    m_data[m_length] = 0;
}

} // namespace iocod
