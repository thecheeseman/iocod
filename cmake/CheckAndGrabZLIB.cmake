macro(check_and_grab_zlib)
    check_include_files(zlib.h HAVE_ZLIB_H)

    # if we don't have zlib, download it from git... assuming we have git
    if (NOT HAVE_ZLIB_H)
        #configure_file(dependencies/zlib.txt.in zlib-download/CMakeLists.txt)

        #execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        #    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/zlib-download")
        #execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        #    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/zlib-download")

        #add_subdirectory("${CMAKE_BINARY_DIR}/zlib-src" 
        #    "${CMAKE_BINARY_DIR}/zlib-build")
    endif()

endmacro()
