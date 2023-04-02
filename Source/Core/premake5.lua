-- SPDX-FileCopyrightText: 2023 thecheeseman
--
-- SPDX-License-Identifier: CC0-1.0

project "iocodcore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    files { "**.h", "**.cpp" }

    -- pchheader "Include/iocod_pch.h"

    includedirs { "%{wks.location}/Source/Core/Include" }