// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <eastl_iocod.h>
#include <EASTL/internal/config.h>
#include <EASTL/allocator.h>

//
// custom allocator for eastl
//

namespace eastl {

allocator g_defaultAllocator;

allocator* GetDefaultAllocator()
{
    return &g_defaultAllocator;
}

allocator* SetDefaultAllocator(allocator*)
{
    return &g_defaultAllocator;
}

allocator::allocator(const char* EASTL_NAME(pName)) : mpName(pName) {}
allocator::allocator(const allocator& EASTL_NAME(x)) = default;
allocator::allocator(const allocator&, const char* EASTL_NAME(pName)) : mpName(pName) {}
allocator& allocator::operator=(const allocator& EASTL_NAME(x)) { return *this; }

const char* allocator::get_name() const { return EASTL_ALLOCATOR_DEFAULT_NAME; }
void allocator::set_name(const char* EASTL_NAME(pName)) {}

void* allocator::allocate(const size_t n, int flags)
{
    return iocod::Memory::Alloc(n);
}

void* allocator::allocate(const size_t n, const size_t alignment, const size_t offset, int flags)
{
    return iocod::Memory::Alloc(n, alignment);
}

void allocator::deallocate(void* p, size_t)
{
    iocod::Memory::Free(p);
}

} // namespace eastl
