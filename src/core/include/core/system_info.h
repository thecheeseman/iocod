// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SYSTEM_INFO_H
#define CORE_SYSTEM_INFO_H

#include <core/types.h>
#include <string_view>

namespace iocod {

struct SystemInfo {
    f32 cpu_mhz;
    i32 cpu_cores;
    i32 cpu_threads;
    std::string_view cpu_vendor;
    std::string_view cpu_model;

    u64 mem_total;
    u64 mem_free;
    u64 mem_available;

    u64 mem_virtual_size;
    u64 mem_virtual_available;
    u64 mem_virtual_peak;
    u64 mem_physical_size;
    u64 mem_physical_peak;
};

} // namespace iocod

#endif // CORE_SYSTEM_INFO_H
