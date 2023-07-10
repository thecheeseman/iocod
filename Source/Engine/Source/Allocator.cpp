// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Allocator.h>

namespace iocod {

// --------------------------------
// Allocator::AlignAddress
// --------------------------------
void* Allocator::AlignAddress(void* ptr, const std::size_t alignment)
{
    const auto currentAddress = reinterpret_cast<uintptr_t>(ptr);
    const auto mask = alignment - 1;
    const auto offset = alignment - (currentAddress & mask);
    Assert(offset <= alignment);

    const auto alignedAddress = currentAddress + offset;
    auto alignedPtr = reinterpret_cast<void*>(alignedAddress);

    Assert(reinterpret_cast<uintptr_t>(alignedPtr) % alignment == 0);

    return alignedPtr;
}

// --------------------------------
// DefaultAllocator::Allocate
// --------------------------------
void* DefaultAllocator::Allocate(const std::size_t size)
{
    Assert(size % Memory::kDefaultAlignment == 0);

    return Memory::Alloc(size);
}

// --------------------------------
// DefaultAllocator::Deallocate
// --------------------------------
void DefaultAllocator::Deallocate(void* ptr)
{
    Memory::Free(ptr);
}

// --------------------------------
// SingleFrameAllocator::SingleFrameAllocator
// --------------------------------
SingleFrameAllocator::SingleFrameAllocator(Allocator& baseAllocator, const std::size_t size) :
    m_baseAllocator(baseAllocator),
    m_totalSize(size),
    m_offset(0),
    m_requireMore(false)
{
    void* ptr = m_baseAllocator.Allocate(size);

    Assert(ptr);
    Assert(reinterpret_cast<uintptr_t>(ptr) % Memory::kDefaultAlignment == 0);

    m_start = static_cast<char*>(ptr);
}

// --------------------------------
// SingleFrameAllocator::~SingleFrameAllocator
// --------------------------------
SingleFrameAllocator::~SingleFrameAllocator()
{
    m_baseAllocator.Deallocate(m_start);
}

// --------------------------------
// SingleFrameAllocator::Allocate
// --------------------------------
void* SingleFrameAllocator::Allocate(const std::size_t size)
{
    std::lock_guard lock(m_mutex);

    const std::size_t newSize = size + Memory::kDefaultAlignment;

    if (m_offset + newSize > m_totalSize) {
        m_requireMore = true;
        return m_baseAllocator.Allocate(size);
    }

    void* nextAvailable = m_start + m_offset;
    nextAvailable = AlignAddress(nextAvailable, Memory::kDefaultAlignment);
    m_offset += newSize;

    Assert(reinterpret_cast<uintptr_t>(nextAvailable) % Memory::kDefaultAlignment == 0);

    return nextAvailable;
}

// --------------------------------
// SingleFrameAllocator::Deallocate
// --------------------------------
void SingleFrameAllocator::Deallocate(void* ptr)
{
    std::lock_guard lock(m_mutex);

    // not allocated by this allocator
    if (ptr < m_start || ptr > m_start + m_totalSize)
        m_baseAllocator.Deallocate(ptr);
}

// --------------------------------
// SingleFrameAllocator::Reset
// --------------------------------
void SingleFrameAllocator::Reset()
{
    std::lock_guard lock(m_mutex);

    if (m_requireMore) {
        m_baseAllocator.Deallocate(m_start);
        m_totalSize *= 2;

        m_start = static_cast<char*>(m_baseAllocator.Allocate(m_totalSize));
        Assert(m_start);

        m_requireMore = false;
    }

    m_offset = 0;
}

} // namespace iocod
