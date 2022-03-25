add_library(iocodtest "")
include_directories("include")
add_subdirectory("testlib")
target_sources(iocodtest PRIVATE "../src/shared.c")

# tests
add_subdirectory("logger")
