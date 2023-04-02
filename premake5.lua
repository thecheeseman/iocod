-- SPDX-FileCopyrightText: 2023 thecheeseman
--
-- SPDX-License-Identifier: CC0-1.0

workspace "iocod"
    configurations { "debug", "release" }
    platforms { "x86", "x86_64" }

    filter "system:Linux"
        system "linux"

    filter "platforms:x86"
        defines { "IOCOD_ARCH=i386", "IOCOD_ARCH_I386" }
        architecture "x86"

    filter "platforms:x86_64"
        defines { "IOCOD_ARCH=amd64", "IOCOD_ARCH_AMD64" }
        architecture "x86_64"

    filter "configurations:Debug"
        defines { "DEBUG", "IOCOD_DEBUG" }
        symbols "On"        -- -g
        optimize "Debug"    -- -Og

    filter "configurations:Release"
        defines { "NDEBUG" }
        symbols "On"        -- -g
        optimize "On"       -- -O2

    filter {}

    -- Output directories
    targetdir ("%{wks.location}/build/bin/%{prj.name}")
    objdir ("%{wks.location}/build/obj/%{prj.name}")

    -- Disable "lib" prefix on linux
    targetprefix ""

    -- force all projects to use fastmath
    floatingpoint "Fast"

    -- enable all warnings for all targets
    enablewarnings { "all", "extra" }

group "Core"
    include "Source/Core"
group ""

group "Console"
    include "Source/Core"
    include "Source/Server"
group ""
