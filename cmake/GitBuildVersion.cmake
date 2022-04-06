#
# if we're building from a git clone, then we can use git versioning
# if we don't have git available, then assuming we downloaded from
# the repo anyway, the build_number header probably already exists
#
macro(git_build_version)
    find_package(Git)
    if (Git_FOUND)
        # grab the total number of commits since the 'build' tag (first tag)
        execute_process(COMMAND ${GIT_EXECUTABLE} describe --match build
            OUTPUT_VARIABLE DESCRIBE_BUILD OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE DESCRIBE_RESULT)

        if (DESCRIBE_RESULT EQUAL 0)
            # match git describe (build-55-gd456da2)
            string(REGEX MATCHALL "^build-([0-9]*)-([a-zA-Z0-9]*)" BUILDDATA ${DESCRIBE_BUILD})
            if (CMAKE_MATCH_COUNT GREATER_EQUAL 2)
                set(BUILD_NUMBER "${CMAKE_MATCH_1}")
                set(BUILD_HASH "${CMAKE_MATCH_2}")
            endif()

            # grab the latest tag
            execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --tags
                OUTPUT_VARIABLE DESCRIBE_BUILD OUTPUT_STRIP_TRAILING_WHITESPACE)
            string(REGEX MATCHALL "^v?(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)-?([a-zA-Z0-9]*)?-?([a-zA-Z0-9]*)?-?([a-zA-Z0-9]*)?" BUILDVERSION ${DESCRIBE_BUILD})

            if (CMAKE_MATCH_COUNT GREATER_EQUAL 3)
                # we will always have major.minor.patch
                # but might have more info than that
                set(VERSION_MAJOR "${CMAKE_MATCH_1}")
                set(VERSION_MINOR "${CMAKE_MATCH_2}")
                set(VERSION_PATCH "${CMAKE_MATCH_3}")

                # we might have more information here
                if (CMAKE_MATCH_COUNT GREATER_EQUAL 4)
                    string(COMPARE EQUAL ${CMAKE_MATCH_${CMAKE_MATCH_COUNT}} ${BUILD_HASH} HAS_HASH)

                    if (NOT HAS_HASH OR (HAS_HASH AND CMAKE_MATCH_COUNT EQUAL 6))
                        set(VERSION_DESCRIPTOR "${CMAKE_MATCH_4}")
                    endif()
                endif()
            else()
                message(WARNING "Malformed version tag")
            endif()

            set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

            # spit out to header
            file(WRITE "src/include/build_number.h" 
"/* This file is autogenerated by CMake */\n\n \
#ifndef INCLUDE_BUILD_NUMBER_H\n \
#define INCLUDE_BUILD_NUMBER_H\n\n \
#define IOCOD_BUILD_NUMBER \"${BUILD_NUMBER}\"\n \
#define IOCOD_BUILD_HASH \"${BUILD_HASH}\"\n \
#define IOCOD_VERSION_MAJOR ${VERSION_MAJOR}\n \
#define IOCOD_VERSION_MINOR ${VERSION_MINOR}\n \
#define IOCOD_VERSION_PATCH ${VERSION_PATCH}\n \
#define IOCOD_VERSION_STRING \"${VERSION_STRING}\"\n \
#define IOCOD_VERSION_DESCRIPTOR \"${VERSION_DESCRIPTOR}\"\n \
#define IOCOD_VERSION ((IOCOD_VERSION_MAJOR * 1000000) + (IOCOD_VERSION_MINOR * 1000) + IOCOD_VERSION_PATCH)\n \
#define IOCOD_BUILD_INFO \"v\" IOCOD_VERSION_STRING \"-\" IOCOD_BUILD_NUMBER \"-\" IOCOD_BUILD_HASH \"\"\n \ 
#endif /* INCLUDE_BUILD_NUMBER_H */")
        endif()
    endif()
endmacro()
