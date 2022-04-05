#
# check whether we're building for 64-bit or 32-bit
# if the toolchain isn't specified (i.e. COMPILE_32_BIT isn't defined)
# then let's just make sure we're actually on a 64-bit system
#

macro(check_64_bit)
    if (NOT CMAKE_REQUIRED_QUIET)
        message(STATUS "Checking size of void pointer")
    endif()

    set(COMPILE_64_BIT FALSE)

    if (NOT COMPILE_32_BIT)
        check_c_source_compiles("
        #include <stdlib.h>

        int main(void)
        {
            if (sizeof(void *) == 8)
                return 1;

            return 0;
        }" ON_64_BIT_SYSTEM)

        if (ON_64_BIT_SYSTEM)
            message(STATUS "Building for 64-bit")
            set(COMPILE_64_BIT TRUE)
        endif()
    else()
        message(STATUS "Building for 32-bit")
    endif()
endmacro()
