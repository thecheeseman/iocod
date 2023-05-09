# SPDX-FileCopyrightText: 2022-2100 Amin Yahyaabadi 
# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: MIT
# SPDX-License-Identifier: GPL-3.0-or-later

include_guard()

function(EnableSanitizers _project_name)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(SANITIZERS "")

        if (ENABLE_SANITIZER_ADDRESS)
            list(APPEND SANITIZERS "address")
        endif()

        if (ENABLE_SANITIZER_LEAK)
            list(APPEND SANITIZERS "leak")
        endif()

        if (ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
            list(APPEND SANITIZERS "undefined")
        endif()

        if (ENABLE_SANITIZER_THREAD)
            if ("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
                message(WARNING "Thread sanitizer does not work with Address and Leak sanitizer enabled")
            else()
                list(APPEND SANITIZERS "thread")
            endif()
        endif()

        if (ENABLE_SANITIZER_MEMORY AND CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
            message(
                WARNING
                "Memory sanitizer requires all the code (including libc++) to be MSan-instrumented otherwise it reports false positives"
            )
            if ("address" IN_LIST SANITIZERS OR "thread" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
                message(WARNING "Memory sanitizer does not work with Address, Thread and Leak sanitizer enabled")
            else()
                list(APPEND SANITIZERS "memory")
            endif()
        endif()
    elseif (MSVC)
        if (ENABLE_SANITIZER_ADDRESS)
            list(APPEND SANITIZERS "address")
        endif()
        
        if (${ENABLE_SANITIZER_LEAK}
            OR ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
            OR ${ENABLE_SANITIZER_THREAD}
            OR ${ENABLE_SANITIZER_MEMORY})
            message(WARNING "MSVC only supports address sanitizer")
        endif()
    endif()

    list(
        JOIN
        SANITIZERS
        ","
        LIST_OF_SANITIZERS
    )

    if(LIST_OF_SANITIZERS)
        if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
            if (NOT MSVC)
                target_compile_options(${_project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
                target_link_options(${_project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
            else()
                string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
                
                if ("${index_of_vs_install_dir}" STREQUAL "-1")
                    message(SEND_ERROR "Using MSVC sanitizers requires setting the MSVC environment before building the project. Please manually open the MSVC command prompt and rebuild the project.")
                endif()
                
                target_compile_options(${_project_name} INTERFACE /fsanitize=${LIST_OF_SANITIZERS} /Zi /INCREMENTAL:NO)
                target_link_options(${_project_name} INTERFACE /INCREMENTAL:NO)
            endif()
        endif()
    endif()
endfunction()
