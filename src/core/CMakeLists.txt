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

include_directories("${STAGED_INSTALL_PREFIX}/include")
include_directories("../include")
include_directories(${PROJECT_BINARY_DIR})

link_directories("${STAGED_INSTALL_PREFIX}/lib")

message(STATUS "${CMAKE_INCLUDE_PATH}")

add_library(iocod_core STATIC ${IC_CORE_SRCS})

add_subdirectory("tests")
