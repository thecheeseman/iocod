project(iccore C)

file(GLOB IC_CORE_SRCS "*.c")

foreach (subdir
    config
    cvar
    events
    network
    parse
    shared
    strings)
    file(GLOB sources "${subdir}/*.c")
    list(APPEND IC_CORE_SRCS ${sources})
endforeach()

include_directories("${CMAKE_SOURCE_DIR}/external/libconfuse/src")
include_directories(".")
include_directories(${PROJECT_BINARY_DIR})

add_library(iccore STATIC $<TARGET_OBJECTS:confuse> ${IC_CORE_SRCS})

add_subdirectory("tests")
