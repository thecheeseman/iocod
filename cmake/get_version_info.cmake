macro(get_version_info)
    message(CHECK_START "Getting version info from <iocod/version.h>")

    file(STRINGS 
        ${CMAKE_CURRENT_LIST_DIR}/src/include/iocod/version.h 
        _version_lines REGEX "^#define IC_VERSION_STRING "
    )

    set(_errored FALSE)
    if (NOT ${_version_lines} STREQUAL "")
        string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)"
            _version_matches "${_version_lines}"
        )

        if (_version_matches STREQUAL "")
            message(SEND_ERROR 
                "${Red}Could not regex match version string from "
                "<iocod/version.h>${ColorReset}")
            set(_errored TRUE)
        endif()
    else()
        message(SEND_ERROR 
            "${Red}Could not get version info from <iocod/version.h>. This "
            "could be because the file doesn't exist or the version string "
            "isn't defined in the file.${ColorReset}")
        set(_errored TRUE)
    endif()

    if (_errored)
        set(CMAKE_MATCH_1 "0")
        set(CMAKE_MATCH_2 "0")
        set(CMAKE_MATCH_3 "0")
    endif()

    set(_version_string "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")

    if (_errored)
        message(CHECK_FAIL "failed")
    else()
        message(CHECK_PASS "${_version_string}")
    endif()
endmacro()
