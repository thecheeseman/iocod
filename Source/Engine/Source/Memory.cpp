// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <rpmalloc/rpmalloc.h>
#include <Memory.h>

#ifdef IOCOD_WINDOWS
#include <crtdbg.h>
#endif

namespace iocod::Memory {

static bool g_memoryInitialized = false;
static rpmalloc_config_t g_rpmallocConfig;

// --------------------------------
// Memory::Initialize
// --------------------------------
void Initialize()
{
    Assert(!g_memoryInitialized);

    memset(&g_rpmallocConfig, 0, sizeof(g_rpmallocConfig));
    g_rpmallocConfig.error_callback = [](const char* message) {
        Assert(false, "rpmalloc error: %s", message);
    };

    rpmalloc_initialize_config(&g_rpmallocConfig);

    g_memoryInitialized = true;
}

// --------------------------------
// Memory::Shutdown
// --------------------------------
void Shutdown()
{
    Assert(g_memoryInitialized);
    g_memoryInitialized = false;

    rpmalloc_finalize();
}

// --------------------------------
// Memory::GetTotalRequestedMemory
// --------------------------------
std::size_t GetTotalRequestedMemory()
{
    rpmalloc_global_statistics_t stats;
    rpmalloc_global_statistics(&stats);
    return stats.mapped;
}

// --------------------------------
// Memory::GetTotalAllocatedMemory
// --------------------------------
std::size_t GetTotalAllocatedMemory()
{
    rpmalloc_global_statistics_t stats;
    rpmalloc_global_statistics(&stats);
    return stats.mapped_total;
}

// --------------------------------
// Memory::Alloc
// --------------------------------
void* Alloc(const std::size_t size, const std::size_t alignment)
{
    Assert(g_memoryInitialized);

    if (size == 0)
        return nullptr;

    void* ptr = rpaligned_alloc(alignment, size);
    Assert(IsAligned(ptr, alignment));

    return ptr;
}

// --------------------------------
// Memory::Realloc
// --------------------------------
void* Realloc(void* ptr, const std::size_t size, const std::size_t alignment)
{
    Assert(g_memoryInitialized);

    void* newPtr = rprealloc(ptr, size);
    Assert(newPtr);

    return newPtr;
}

// --------------------------------
// Memory::Free
// --------------------------------
void Free(void* ptr)
{
    Assert(g_memoryInitialized);

    rpfree(ptr);
    ptr = nullptr;
}

} // namespace iocod::Memory

//
//
//

void* __cdecl operator new[](const size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return iocod::Memory::Alloc(size);
}

