// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_CONTAINERS_ARRAY_H
#define ENGINE_CONTAINERS_ARRAY_H

// TODO: reverse iterators
template <typename T, size_t N = 1>
class Array {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;

    value_type m_value[N];

    constexpr void Fill(const T& value)
    {
        for (size_t i = 0; i < N; ++i)
            m_value[i] = value;
    }

    constexpr size_type Size() const noexcept { return N; }

    constexpr size_type MaxSize() const noexcept { return N; }

    constexpr bool Empty() const noexcept { return N == 0; }

    constexpr reference operator[](size_t index)
    {
        Assert(index < N);
        return m_value[index];
    }

    constexpr const_reference operator[](size_t index) const
    {
        Assert(index < N);
        return m_value[index];
    }

    constexpr reference Front() { return m_value[0]; }

    constexpr const_reference Front() const { return m_value[0]; }

    constexpr reference Back() { return m_value[N - 1]; }

    constexpr const_reference Back() const { return m_value[N - 1]; }

    constexpr T* Data() noexcept { return m_value; }

    constexpr const T* Data() const noexcept { return m_value; }

    constexpr const_reference At(size_t index) const
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");

        return static_cast<const T&>(m_value[index]);
    }

    constexpr reference At(size_t index)
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");

        return static_cast<T&>(m_value[index]);
    }

    // STL container requirements
    using iterator = T*;
    using const_iterator = const T*;

    constexpr iterator begin() noexcept { return &m_value[0]; }

    constexpr const_iterator begin() const noexcept { return &m_value[0]; }

    constexpr const_iterator cbegin() const noexcept { return &m_value[0]; }

    constexpr iterator end() noexcept { return &m_value[N]; }

    constexpr const_iterator end() const noexcept { return &m_value[N]; }

    constexpr const_iterator cend() const noexcept { return &m_value[N]; }
};

template <typename T, typename... U>
Array(T, U) -> Array<T, 1 + sizeof...(U)>;

#endif // ENGINE_CONTAINERS_ARRAY_H
