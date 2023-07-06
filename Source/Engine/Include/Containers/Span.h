// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_CONTAINERS_SPAN_H
#define ENGINE_CONTAINERS_SPAN_H

namespace iocod {

template <typename T>
class Span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    constexpr Span() :
        m_begin(nullptr), m_end(nullptr), m_size(0) {}

    constexpr Span(pointer begin, size_type end) :
        m_begin(begin), m_end(begin + end), m_size(end) {}

    constexpr Span(pointer begin, pointer end) :
        m_begin(begin), m_end(end), m_size(end - begin) {}

    template <std::size_t N>
    constexpr explicit Span(T (&value)[N]) :
        m_begin(value), m_end(value + N), m_size(N) {}

    constexpr reference operator[](size_type index) const
    {
        Assert(index < m_size);
        return m_begin[index];
    }

    constexpr explicit operator Span<const T>() const
    {
        return Span<const T>(m_begin, m_end);
    }

    [[nodiscard]] constexpr const_pointer Data() const 
    {
        return m_begin;
    }

    [[nodiscard]] constexpr const_reference Back() const
    {
        Assert(m_size > 0);
        return *(m_end - 1);
    }

    [[nodiscard]] constexpr const_reference Front() const
    {
        Assert(m_size > 0);
        return *m_begin;
    }

    [[nodiscard]] constexpr size_type Size() const
    {
        return m_size;
    }

    [[nodiscard]] constexpr size_type Length() const
    {
        return m_size;
    }

    [[nodiscard]] constexpr bool Equals(const Span<T>& rhs) const
    {
        if (m_size != rhs.m_size)
            return false;

        for (size_type i = 0; i < m_size; ++i)
            if (m_begin[i] != rhs.m_begin[i])
                return false;

        return true;
    }

    template <typename FindFunc>
    difference_type Find(FindFunc&& func)
    {
        for (size_type i = 0; i < m_size; ++i)
            if (func(m_begin[i]))
                return i;

        return -1;
    }

    [[nodiscard]] constexpr Span FromLeft(size_type count) const
    {
        Assert(count <= m_size);
        return Span(m_begin, m_begin + count);
    }

    [[nodiscard]] constexpr Span FromRight(size_type count) const
    {
        Assert(count <= m_size);
        return Span(m_end - count, m_end);
    }

    // STL compatibility
    [[nodiscard]] const_pointer begin() const { return m_begin; }
    [[nodiscard]] const_pointer end() const { return m_end; }

private:
    pointer m_begin;
    pointer m_end;
    size_type m_size;
};

} // namespace iocod

#endif // ENGINE_CONTAINERS_SPAN_H
