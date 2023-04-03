# SPDX-FileCopyrightText: 2022-2100 Amin Yahyaabadi 
# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: MIT
# SPDX-License-Identifier: GPL-3.0-or-later

include_guard()

function(SetProjectWarnings _project_name)
    set(CLANG_WARNINGS
        -Wall
        -Wextra                 # reasonable and standard
        -Wpedantic              # warn if non-standard C++ is used

        -Wcast-align            # warn for potential performance problem casts
        -Wcast-qual             # warn when a pointer is cast away from a type qualifier
        -Wctor-dtor-privacy     # warn the user if a class with non-public constructors
        -Wconversion            # warn on type conversions that may lose data
        -Wdisabled-optimization # warn generally if your code is too big / complex
        -Wdouble-promotion      # warn if float is implicit promoted to double
        -Wextra-semi            # Warn about semicolon after in-class function definition.
        -Wformat=2              # warn on security issues around functions that format output (ie printf)
        -Wimplicit-fallthrough  # warn on statements that fallthrough without an explicit annotation
        -Winit-self             # warn if a member variable is initialized with itself
        -Wmissing-declarations  # warn if a global function is defined without a previous declaration
        -Wmissing-include-dirs  # warn if a directory in the include path is missing
        -Wnon-virtual-dtor      # warn the user if a class with virtual functions has a non-virtual destructor
        -Wnull-dereference      # warn if a null dereference is detected
        -Wold-style-cast        # warn for c-style casts
        -Woverloaded-virtual    # warn if you overload (not override) a virtual function
        -Wredundant-decls       # warn if a decl is already declared in a scope
        -Wshadow                # warn the user if a variable declaration shadows one from a parent context
        -Wsign-conversion       # warn on sign conversions
        -Wsign-promo            # warn if implicit conversion changes signedness
        -Wswitch-default        # warn if switch has default case but no other cases
        -Wundef                 # warn if a macro is not defined
        -Wunused                # warn on anything being unused
    )

    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
        -Wduplicated-cond # warn if if / else chain has duplicated conditions
        -Wduplicated-branches # warn if if / else branches have duplicated code
        -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
        -Wuseless-cast # warn if you perform a cast to the same type
    )

    if (WARNINGS_AS_ERRORS)
        message(TRACE "Warnings are treated as errors")
        list(APPEND CLANG_WARNINGS -Werror)
        list(APPEND GCC_WARNINGS -Werror)
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
    else()
        message(WARNING "Unknown compiler ${CMAKE_CXX_COMPILER_ID}, no warnings are set")
    endif()

    set(PROJECT_WARNINGS_C ${PROJECT_WARNINGS_CXX})
    list(
        REMOVE_ITEM
        PROJECT_WARNINGS_C
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wuseless-cast
        -Wextra-semi
    )

    target_compile_options(
        ${_project_name}
        INTERFACE
        $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS_CXX}>
        $<$<COMPILE_LANGUAGE:C>:${PROJECT_WARNINGS_C}>
    )
endfunction()