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

    set(MSVC_WARNINGS
        /W4 # Baseline reasonable warnings
        /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
        /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
        /w14263 # 'function': member function does not override any base class virtual member function
        /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
                # be destructed correctly
        /w14287 # 'operator': unsigned/negative constant mismatch
        /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
                # the for-loop scope
        /w14296 # 'operator': expression is always 'boolean_value'
        /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545 # expression before comma evaluates to a function which is missing an argument list
        /w14546 # function call before comma missing argument list
        /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
        /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
        /w14555 # expression has no effect; expected expression with side- effect
        /w14619 # pragma warning: there is no warning number 'number'
        /w14640 # Enable warning on thread un-safe static member initialization
        /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
        /w14905 # wide string literal cast to 'LPSTR'
        /w14906 # string literal cast to 'LPWSTR'
        /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
        /permissive- # standards conformance mode for MSVC compiler.

        #/w44365 # 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
        /w44388 # signed/unsigned mismatch
        /w14548 # expression before comma has no effect; expected expression with side-effect
        /w15219 # implicit conversion from 'type-1' to 'type-2', possible loss of data
        /w15233 # explicit lambda capture 'identifier' is not used
        /w15262 # implicit fall-through occurs here; are you missing a break statement?
        /w15263 # calling 'std::move' on a temporary object prevents copy elision

        /w44710
        /w44711
    )

    if (WARNINGS_AS_ERRORS)
        message(TRACE "Warnings are treated as errors")
        list(APPEND CLANG_WARNINGS -Werror)
        list(APPEND GCC_WARNINGS -Werror)
        list(APPEND MSVC_WARNINGS /WX)
    endif()

    if (MSVC)
        set(PROJECT_WARNINGS_CXX ${MSVC_WARNINGS})
    elseif (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
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
