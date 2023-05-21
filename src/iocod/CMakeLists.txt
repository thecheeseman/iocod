# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(IOCOD_SOURCES
    main.cpp
)
list(TRANSFORM IOCOD_SOURCES PREPEND "src/")

#
# iocod executable
#
add_executable(iocod ${IOCOD_SOURCES})
add_dependencies(iocod iocodcore)

#if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    set_target_properties(iocod PROPERTIES WIN32_EXECUTABLE TRUE)
#elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
#    set_target_properties(iocod PROPERTIES MACOSX_BUNDLE TRUE)
#endif()

target_link_libraries(iocod PRIVATE iocod_options iocod_warnings iocod_includes)
target_link_libraries(iocod PRIVATE iocodcore)
target_include_directories(iocod PRIVATE "src/" "include/")

# remove the "lib" prefix from the library name
set_target_properties(iocod PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)

