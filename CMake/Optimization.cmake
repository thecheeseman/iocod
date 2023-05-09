# SPDX-FileCopyrightText: 2022-2100 Amin Yahyaabadi 
# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: MIT
# SPDX-License-Identifier: GPL-3.0-or-later

include_guard()

macro(EnableInterproceduralOptimization _project_name)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)

    if (result)
        # If a static library of this project is used in another project that does not have `CMAKE_INTERPROCEDURAL_OPTIMIZATION` enabled, a linker error might happen.
        # TODO set this option in `package_project` function.
        message(
        STATUS
            "Interprocedural optimization is enabled. In other projects, linking with the compiled libraries of this project might require `set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)`"
        )
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
        set_target_properties(${_project_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(WARNING "Interprocedural Optimization is not supported: ${output}")
    endif()
endmacro()
