// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

namespace iocod {

namespace Memory {

inline constexpr std::size_t kDefaultAlignment = 16;

IOCOD_API void Initialize();
IOCOD_API void Shutdown();

IOCOD_API std::size_t GetTotalRequestedMemory();
IOCOD_API std::size_t GetTotalAllocatedMemory();

IOCOD_API [[nodiscard]] void* Alloc(std::size_t size, std::size_t alignment = kDefaultAlignment);
IOCOD_API [[nodiscard]] void* Realloc(void* ptr, std::size_t size, std::size_t alignment = kDefaultAlignment);
IOCOD_API void Free(void* ptr);

//
// memory utils
//

IOCOD_ALWAYS_INLINE void MemsetZero(void* ptr, const std::size_t size)
{
    std::memset(ptr, 0, size);
}

template <typename T>
IOCOD_ALWAYS_INLINE void MemsetZero(T* ptr)
{
    std::memset(ptr, 0, sizeof(T));
}

inline bool IsAligned(const void* p, const std::size_t n)
{
    return reinterpret_cast<std::uintptr_t>(p) % n == 0;
}

template <typename T>
bool IsAligned(const T* p)
{
    return reinterpret_cast<std::uintptr_t>(p) % alignof(T) == 0;
}

} // namespace Memory

} // namespace iocod

#endif // ENGINE_MEMORY_H
