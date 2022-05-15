project(core_tests)

add_executable(test-logger "test-logger.c")
target_link_libraries(test-logger iccore)
