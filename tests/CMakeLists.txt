project(tests)

if (WIN32)
    include_directories("${CMAKE_SOURCE_DIR}/external/dlfcn-win32")
    include_directories("${CMAKE_SOURCE_DIR}/external/dirent")
endif()

include_directories("${CMAKE_SOURCE_DIR}/external/miniz")

#add_subdirectory(filesystem)
add_subdirectory(icheaders)
#add_subdirectory(dlfcn)
#add_subdirectory(dirent)

add_subdirectory(fsnew)
