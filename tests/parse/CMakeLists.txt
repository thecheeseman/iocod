add_executable(test_parse "parse.c" "../../src/parse.c")
set_target_properties(test_parse PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/parse/"
)
target_link_libraries(test_parse PRIVATE iocodtest)