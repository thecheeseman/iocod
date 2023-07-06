// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdexcept>
#include <Core/String.h>

namespace iocod {

// --------------------------------
// String::Reserve
// --------------------------------
void String::Reserve(const size_type newCapacity)
{
    std::size_t newSize = newCapacity;
    if (const std::size_t mod = newCapacity % kGranularity; mod)
        newSize = newCapacity + kGranularity - mod;

    m_allocated = newSize;

    // TODO: optionally use a custom allocator here
    const auto newBuffer = static_cast<char*>(Memory::Alloc(newSize));
    //

    m_data[m_length] = 0;
    Copy(newBuffer, m_data);

    if (m_data && m_data != m_baseBuffer)
        Memory::Free(m_data);

    m_data = newBuffer;
}

// --------------------------------
// String::String
// --------------------------------
constexpr String& String::operator=(const String& other)
{
    if (this != &other) {
        const std::size_t length = other.m_length;
        if (length >= m_allocated)
            Reserve(length + 1);

        memcpy(m_data, other.m_data, length);
        m_data[length] = 0;
        m_length = length;
    }

    return *this;
}

constexpr String& String::operator=(String&& other) noexcept
{
    if (this != &other) {
        if (m_data != m_baseBuffer)
            Memory::Free(m_data);

        if (other.m_data == other.m_baseBuffer) {
            memcpy(m_baseBuffer, other.m_baseBuffer, other.m_length);
            m_baseBuffer[other.m_length] = 0;
            m_data = m_baseBuffer;
        } else {
            m_data = other.m_data;
        }

        m_length = other.m_length;
        m_allocated = other.m_allocated;

        other.m_allocated = 0;
        other.m_data = nullptr;
        other.m_baseBuffer[0] = 0;
        other.m_length = 0;
    }

    return *this;
}

constexpr String& String::operator=(const std::string& other)
{
    const std::size_t length = other.length();
    if (length >= m_allocated)
        Reserve(length + 1);

    std::ranges::copy(other.begin(), other.end(), m_data);
    m_data[length] = 0;
    m_length = length;
    return *this;
}

constexpr String& String::operator=(std::string&& other) noexcept
{
    const std::size_t length = other.length();
    if (length >= m_allocated)
        Reserve(length + 1);

    std::ranges::move(other.begin(), other.end(), m_data);
    m_data[length] = 0;
    m_length = length;
    return *this;
}

String::String(const wchar_t* s, size_type count)
{
    Assert(s);

    auto state = std::mbstate_t();
    const std::size_t len = std::wcsrtombs(nullptr, &s, 0, &state);

    if (count > len)
        count = len;

    if (len > m_allocated)
        Reserve(len + 1);

    // TODO: check conversion errors
    const std::size_t written = std::wcsrtombs(&m_data[0], &s, len, &state);
    Assert(written == count);
    m_data[count] = 0;
    m_length = count;
}

String::String(const i32 i)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%d", i);

    if (len > m_allocated)
        Reserve(len);

    Copy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

String::String(const i64 i)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%lld", i);

    if (len > m_allocated)
        Reserve(len);

    Copy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

String::String(const u32 u)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%u", u);

    if (len > m_allocated)
        Reserve(len);

    Copy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

String::String(const u64 u)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%llu", u);

    if (len > m_allocated)
        Reserve(len);

    Copy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

String::String(const f32 f)
{
    char buffer[64]{};

    std::size_t len = sprintf_s(buffer, "%f", f);

    while (len > 0 && buffer[len - 1] == '0')
        buffer[--len] = 0;

    while (len > 0 && buffer[len - 1] == '.')
        buffer[--len] = 0;

    if (len > m_allocated)
        Reserve(len);

    Copy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

String::String(const f64 f)
{
    char buffer[64]{};

    std::size_t len = sprintf_s(buffer, "%f", f);

    while (len > 0 && buffer[len - 1] == '0')
        buffer[--len] = 0;

    while (len > 0 && buffer[len - 1] == '.')
        buffer[--len] = 0;

    if (len > m_allocated)
        Reserve(len);

    Copy(m_data, buffer);
    m_data[len] = 0;
    m_length = len;
}

// --------------------------------
// String::~String
// --------------------------------
String::~String()
{
    if (m_data && m_data != m_baseBuffer)
        Memory::Free(m_data);
}

// --------------------------------
// String::Clear
// --------------------------------
constexpr void String::Clear() noexcept
{
    if (m_data && m_data != m_baseBuffer) {
        Memory::Free(m_data);
        m_data = m_baseBuffer;
    }

    for (auto c : m_baseBuffer)
        c = 0;

    m_data[0] = 0;
    m_length = 0;
    m_allocated = kBaseSize;
}

// --------------------------------
// String::WideStringSize
// --------------------------------
String::size_type String::WideStringSize() const noexcept
{
    return mbstowcs(nullptr, m_data, 0);
}

// --------------------------------
// String::operator+=
// --------------------------------
String& String::operator+=(const i32 i)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%d", i);
    Append(buffer, len);
    return *this;
}

String& String::operator+=(const i64 i)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%lld", i);
    Append(buffer, len);
    return *this;
}

String& String::operator+=(const u32 i)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%u", i);
    Append(buffer, len);
    return *this;
}

String& String::operator+=(const u64 u)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%llu", u);
    Append(buffer, len);
    return *this;
}

// weird behavior from doom3:
// on FString(f32) constructor, converts float and removes trailing '0' and '.' if needed
// but here it doesn't do this
// TODO: consolidate/fix this??
String& String::operator+=(const f32 f)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%f", f);
    Append(buffer, len);
    return *this;
}

String& String::operator+=(const f64 f)
{
    char buffer[64]{};

    const std::size_t len = sprintf_s(buffer, "%f", f);
    Append(buffer, len);
    return *this;
}

//
// static methods
//

void String::ToWideString(const char* str, wchar_t* dest, const size_type destSize)
{
    Assert(dest);

    auto state = std::mbstate_t();
    const std::size_t len = std::mbsrtowcs(nullptr, &str, destSize, &state);
    const std::size_t written = std::mbsrtowcs(&dest[0], &str, len, &state);
}

} // namespace iocod
