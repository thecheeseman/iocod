// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

#include <Core/ClassNonCopyable.h>
#include <Core/Platform.h>
#include <Core/SourceLocation.h>
#include <Core/Types.h>

namespace iocod {

namespace Memory {

/**
 * \brief Allocate memory of a given size, aligned to 16 bytes.
 * \param[in] size the size of the memory to allocate
 * \param[in] location the location of the allocation
 * \return a pointer to the allocated memory or nullptr if the allocation failed
 */
IOCOD_EXPORT constexpr void* Alloc(std::size_t size, SourceLocation location = SourceLocation::Current());

/**
 * \brief Free memory allocated with Alloc.
 * \param[in] ptr a pointer to the memory to free
 */
IOCOD_EXPORT constexpr void Free(void* ptr);

} // namespace iocod::Memory

/**
 * \brief Stack-based aligned linear allocator.
 * \tparam Size the size of the allocator
 * \tparam Alignment the alignment of the allocator
 */
template <std::size_t Size, std::size_t Alignment = 16>
class StaticAllocator {
public:
    CLASS_NON_COPYABLE(StaticAllocator)

    StaticAllocator()
    {
        static_assert(Size % Alignment == 0, "Size must be a multiple of Alignment");
    }

    [[nodiscard]] void* Alloc(const std::size_t size) noexcept
    {
        const std::size_t alignedSize = Pow2AlignUp(size, Alignment);
        if (m_offset + alignedSize > Size)
            return nullptr;

        auto* ptr = m_data + m_offset;
        m_offset += alignedSize;
        return ptr;
    }

    void Free([[maybe_unused]] void* ptr) noexcept {}

    void Reset() noexcept { m_offset = 0; }

private:
    std::byte m_data[Size]{};
    std::size_t m_offset = 0;
};

} // namespace iocod

#endif // CORE_MEMORY_H
