if (WIN32)
    add_subdirectory("dlfcn-win32")
    add_subdirectory("getopt")
endif()

add_subdirectory("libconfuse")
add_subdirectory("miniz")
