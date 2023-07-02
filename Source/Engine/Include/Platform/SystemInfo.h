// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_PLATFORM_SYSTEM_INFO_H
#define ENGINE_PLATFORM_SYSTEM_INFO_H

#include <Core/String.h>

namespace iocod {

struct SystemInfo {
    f32 cpuMhz{};
    i32 cpuCores{};
    i32 cpuThreads{};
    String cpuVendor{};
    String cpuModel{};

    u64 memTotal{};
    u64 memFree{};
    u64 memAvailable{};

    u64 memVirtualSize{};
    u64 memVirtualAvailable{};
    u64 memVirtualPeak{};
    u64 memPhysicalSize{};
    u64 memPhysicalPeak{};
};

} // namespace iocod

#endif // ENGINE_PLATFORM_SYSTEM_INFO_H
