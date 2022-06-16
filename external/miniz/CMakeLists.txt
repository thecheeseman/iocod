find_package(miniz 1 CONFIG QUIET)

if (miniz_FOUND)
    add_library(miniz_external INTERFACE) # dummy
else()
    include(ExternalProject)
    ExternalProject_Add(miniz_external
        GIT_REPOSITORY https://github.com/richgel999/miniz
        GIT_TAG cb97387d20909c0a4728972fd0f19bfacd083c42

        DOWNLOAD_NO_PROGRESS ON
        UPDATE_COMMAND ""
        UPDATE_DISCONNECTED TRUE

        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS:BOOL=OFF # only build statically
            -DAMALGAMATE_SOURCES:BOOL=ON # combine into one header
    )
endif()
