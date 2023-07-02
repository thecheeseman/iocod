// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <core/memory.h>

#ifdef IOCOD_WINDOWS
#include <crtdbg.h>
#endif

namespace iocod::Memory {

constexpr std::size_t kAlignment = 16;

constexpr void* Alloc(const std::size_t size, const SourceLocation location)
{
    if (!size)
        return nullptr;

    const std::size_t paddedSize = Pow2AlignUp(size, kAlignment);

    #ifdef IOCOD_WINDOWS
    return _aligned_malloc_dbg(paddedSize, kAlignment, location.FileName(), location.Line());
    #else
    return aligned_alloc(kAlignment, paddedSize);
    #endif
}

constexpr void Free(void* ptr)
{
    if (!ptr)
        return;

    #ifdef IOCOD_WINDOWS
    _aligned_free_dbg(ptr);
    #else
    free(ptr);
    #endif
}

} // namespace iocod::Memory
