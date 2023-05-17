// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/platform.h>
#include <sys/mman.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Main.hpp"

// here to supress missing declarations
extern "C" const char* IOCOD_EXPORT __asan_default_options(); // NOLINT
extern "C" void IOCOD_CONSTRUCTOR _iocod_preload();
extern "C" void IOCOD_DESTRUCTOR _iocod_unload();

// --------------------------------
// __asan_default_options
// --------------------------------
extern "C" const char* IOCOD_EXPORT __asan_default_options() // NOLINT
{
    // address sanitizer default flags
    // https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
    return "verify_asan_link_order=0:detect_leaks=1:detect_stack_use_after_"
           "return=1";
}

// --------------------------------
// _iocod_preload
// --------------------------------
extern "C" void IOCOD_CONSTRUCTOR _iocod_preload()
{
    static bool loaded = false;
    if (loaded)
        return;
    loaded = true;

    unsetenv("LD_PRELOAD");

    const std::size_t length = 0x135000;
    const std::intptr_t OFFSET_COD_LNXDED_START = 0x8048000;
    if (mprotect(reinterpret_cast<void*>(OFFSET_COD_LNXDED_START), length,
                 PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
        std::cerr << "Failed to mprotect cod_lnxded: " << strerror(errno)
                  << "\n";
        std::terminate();
    }

    const std::intptr_t OFFSET_LIBC_START_MAIN = 0x804a698;
    *reinterpret_cast<int*>(OFFSET_LIBC_START_MAIN) =
        reinterpret_cast<std::intptr_t>(_iocod_main);
}

// --------------------------------
// _iocod_unload
// --------------------------------
extern "C" void IOCOD_DESTRUCTOR _iocod_unload()
{
    static bool freed = false;
    if (freed)
        return;
    freed = true;
}
