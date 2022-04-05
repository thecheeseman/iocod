#
# check for attributes
#
macro(check_compiler_attributes)
    if (NOT CMAKE_REQUIRED_QUIET)
        message(STATUS "Checking for compiler attributes")
    endif()

    check_c_source_compiles("
    int main(void) 
    {
        int x __attribute__((unused));
        return 1;
    }" HAVE_ATTRIBUTE_UNUSED)

    check_c_source_compiles("
    int main(void)
    {
        int x __attribute__((deprecated));
        return 1;
    }" HAVE_ATTRIBUTE_DEPRECATED)
endmacro()
