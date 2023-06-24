// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CONTAINERS_ARRAY_H
#define CORE_CONTAINERS_ARRAY_H

// TODO: reverse iterators
template <typename T, size_t N = 1>
class Array {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;

    value_type m_value[N];

    inline void Fill(const T& value)
    {
        for (size_t i = 0; i < N; ++i)
            m_value[i] = value;
    }

    constexpr inline size_type Size() const noexcept { return N; }

    constexpr inline size_type MaxSize() const noexcept { return N; }

    constexpr inline bool Empty() const noexcept { return N == 0; }

    constexpr inline reference operator[](size_t index) { return m_value[index]; }

    constexpr inline const_reference operator[](size_t index) const { return m_value[index]; }

    constexpr inline reference Front() { return m_value[0]; }

    constexpr inline const_reference Front() const { return m_value[0]; }

    constexpr inline reference Back() { return m_value[N - 1]; }

    constexpr inline const_reference Back() const { return m_value[N - 1]; }

    constexpr inline T* Data() noexcept { return m_value; }

    constexpr inline const T* Data() const noexcept { return m_value; }

    constexpr inline const_reference At(size_t index) const
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");

        return static_cast<const T&>(m_value[index]);
    }

    constexpr inline reference At(size_t index)
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");

        return static_cast<T&>(m_value[index]);
    }

    // STL container requirements
    using iterator = T*;
    using const_iterator = const T*;

    constexpr inline iterator begin() noexcept { return &m_value[0]; }

    constexpr inline const_iterator begin() const noexcept { return &m_value[0]; }

    constexpr inline const_iterator cbegin() const noexcept { return &m_value[0]; }

    constexpr inline iterator end() noexcept { return &m_value[N]; }

    constexpr inline const_iterator end() const noexcept { return &m_value[N]; }

    constexpr inline const_iterator cend() const noexcept { return &m_value[N]; }
};

template <typename T, typename... U>
Array(T, U) -> Array<T, 1 + sizeof...(U)>;

#endif // CORE_CONTAINERS_ARRAY_H
