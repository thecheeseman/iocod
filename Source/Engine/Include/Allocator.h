// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_CORE_ALLOCATOR_H
#define ENGINE_CORE_ALLOCATOR_H

#include <mutex>

namespace iocod {

class Allocator {
public:
    Allocator() = default;
    virtual ~Allocator() = default;

    Allocator& operator=(const Allocator&) = default;
    Allocator& operator=(Allocator&&) = default;
    Allocator(const Allocator&) = default;
    Allocator(Allocator&&) = default;

    virtual void* Allocate(std::size_t size) = 0;
    virtual void Deallocate(void* ptr) = 0;

    static void* AlignAddress(void* ptr, std::size_t alignment = Memory::kDefaultAlignment);
};

// DefaultAllocator around rpmalloc. Not thread-safe.
class DefaultAllocator final : public Allocator {
public:
    ~DefaultAllocator() override = default;

    DefaultAllocator(const DefaultAllocator&) = default;
    DefaultAllocator& operator=(const DefaultAllocator&) = default;
    DefaultAllocator(DefaultAllocator&&) = default;
    DefaultAllocator& operator=(DefaultAllocator&&) = default;

    void* Allocate(std::size_t size) override;
    void Deallocate(void* ptr) override;
};

// Single-frame allocator. Thread-safe.
class SingleFrameAllocator final : public Allocator {
public:

    static constexpr std::size_t kDefaultInitialSize = 1024ull * 1024ull;

    /*implicit*/ SingleFrameAllocator(Allocator& baseAllocator, std::size_t size = kDefaultInitialSize);
    ~SingleFrameAllocator() override;

    CLASS_NON_COPYABLE(SingleFrameAllocator)

    void* Allocate(std::size_t size) override;
    void Deallocate(void* ptr) override;

    void Reset();

private:
    Allocator& m_baseAllocator;
    std::mutex m_mutex;
    std::size_t m_totalSize;
    char* m_start;
    std::size_t m_offset;
    bool m_requireMore;
};

} // namespace iocod

#endif // ENGINE_CORE_ALLOCATOR_H
