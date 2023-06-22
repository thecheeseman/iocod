// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/string.h>

namespace iocod {

FString::FString(const char* text)
{
    Construct();

    if (text) {
        const std::size_t length = std::strlen(text);
        EnsureAllocated(length + 1);
        Strcpy(m_data, text);
        m_length = length;
    }
}

void FString::Construct()
{
    SetAllocated(kBaseSize);
    m_data = m_base;
    m_length = 0;
    m_data[0] = '\0';
}

void FString::EnsureAllocated(const std::size_t amount, const bool keepOld)
{
    if (amount > GetAllocated())
        Reallocate(amount, keepOld);
}

void FString::Reallocate(const std::size_t amount, const bool keepOld)
{
    std::size_t newSize;
    if (const std::size_t mod = amount % kGranularity; mod == 0)
        newSize = amount;
    else
        newSize = amount + (kGranularity - mod);

    SetAllocated(newSize);
    const auto newBuffer = new char[GetAllocated()];

    if (keepOld && m_data) {
        m_data[m_length] = '\0';
        Strcpy(newBuffer, m_data);
    }

    if (m_data && m_data != m_base)
        delete[] m_data;

    m_data = newBuffer;
}

void FString::Free()
{
    if (m_data && m_data != m_base) {
        delete[] m_data;

        m_data = m_base;
    }
}

void FString::Clear()
{
    Free();
    Construct();
}

bool FString::IsEmpty() const
{
    return strcmp(m_data, "") == 0;
}

} // namespace iocod
