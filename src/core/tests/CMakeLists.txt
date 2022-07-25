# iocod
# Copyright (C) 2022 thecheeseman
# 
# This file is part of the iocod GPL source code.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(core_tests C CXX)

include(CTest)
add_custom_target(check 
    COMMAND ${CMAKE_CTEST_COMMAND} 
        --output-on-failure -C ${CMAKE_CFG_INTDIR}
)

function(add_core_test)
    set(one_value_args NAME)
    set(multi_value_args SOURCES WIN32_LIBS MACOS_LIBS LINUX_LIBS)
    cmake_parse_arguments(PARSE_ARGV 0 CT
        "" 
        "${one_value_args}" 
        "${multi_value_args}" 
    )
    add_executable(${CT_NAME} ${CT_SOURCES})

    if (NOT IC_PLATFORM_64BIT)
        target_compile_options(${CT_NAME} BEFORE PUBLIC ${IC_PLATFORM_CFLAGS_32BIT})
    endif()

    add_test(NAME ${CT_NAME} 
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${CT_NAME}>
        COMMAND $<TARGET_FILE:${CT_NAME}>)
    add_dependencies(check ${CT_NAME})
    target_link_libraries(${CT_NAME} iocod_core)

    if (IC_PLATFORM_WINDOWS)
        target_link_options(${CT_NAME} BEFORE PUBLIC "/SUBSYSTEM:CONSOLE,5.01")
        target_link_libraries(${CT_NAME} Ws2_32 Dbghelp Psapi)

        if (DEFINED CT_WIN32_LIBS)
            target_link_libraries(${CT_NAME} ${CT_WIN32_LIBS})
        endif()
    endif()

    if (IC_PLATFORM_LINUX)
        target_link_libraries(${CT_NAME} m)
        
        if (DEFINED CT_LINUX_LIBS)
            target_link_libraries(${CT_NAME} ${CT_LINUX_LIBS})
        endif()
    endif()

    if (IC_PLATFORM_MACOS AND DEFINED CT_MACOS_LIBS)
        target_link_libraries(${CT_NAME} ${CT_MACOS_LIBS})
    endif()

    set_target_properties(${CT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/tests/${CT_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/tests/${CT_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/tests/${CT_NAME}")

    message(STATUS "Added iccore test '${CT_NAME}' (${CT_SOURCES})")
endfunction()

# special tests that require more arguments
list(APPEND SPECIAL_TESTS 
    "test-library"
)

file(GLOB TEST_SRCS CONFIGURE_DEPENDS "*.c" "*.cc")
foreach (_test IN LISTS TEST_SRCS)
    # strip file path to get clean test name
    get_filename_component(_name ${_test} NAME)
    string(REPLACE "*.cc" "" _name ${_name})
    string(REPLACE ".c" "" _name ${_name})

    set(_skip FALSE)
    foreach (_spt IN LISTS SPECIAL_TESTS)
        if (${_spt} STREQUAL ${_name})
            set(_skip TRUE)
            break()
        endif()
    endforeach()

    if (NOT _skip)
        add_core_test(NAME ${_name} SOURCES ${_test})
    endif()
endforeach()

add_core_test(NAME "test-library"
    SOURCES "test-library.c"
    LINUX_LIBS "dl"
)
