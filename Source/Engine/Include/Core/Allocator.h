// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_CORE_ALLOCATOR_H
#define ENGINE_CORE_ALLOCATOR_H

namespace iocod {

// base allocator
struct IOCOD_API Allocator {

    using size_type = std::size_t;

    virtual ~Allocator() = default;

    virtual void* Allocate(size_type size) = 0;
    virtual void Deallocate(void* ptr) = 0;
    virtual void* Reallocate(void* ptr, size_type size) = 0;

    template <typename T>
    void DeleteObject(T* ptr)
    {
        if (ptr) {
            ptr->~T();
            Deallocate(ptr);
        }
    }
};

} // namespace iocod

#endif // ENGINE_CORE_ALLOCATOR_H
