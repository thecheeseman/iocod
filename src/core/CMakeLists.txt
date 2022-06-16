cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(iocod_core LANGUAGES C)

file(GLOB IC_CORE_SRCS "*.c")

foreach (subdir
    config
    console
    cvar
    events
    network
    parse
    shared
    strings
    system)
    file(GLOB sources "${subdir}/*.c")
    list(APPEND IC_CORE_SRCS ${sources})
endforeach()

if (IC_PLATFORM_WINDOWS)
    # silence some warnings
    add_compile_options(/wd4244) # conversion from 'double' to 'float'
    add_compile_options(/wd4305) # truncation from 'double' to 'float'
    add_compile_options(/wd4996) # unsafe functions strcpy etc
endif()

add_compile_definitions(HAVE_CONFIG_H)
include_directories("${STAGED_INSTALL_PREFIX}/include")
include_directories("../include")
include_directories(${PROJECT_BINARY_DIR})

link_directories("${STAGED_INSTALL_PREFIX}/lib")

message(STATUS "${CMAKE_INCLUDE_PATH}")

add_library(iocod_core STATIC ${IC_CORE_SRCS})

add_subdirectory("tests")
