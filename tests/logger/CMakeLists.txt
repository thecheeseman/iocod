add_executable(test_logger "logger.c" "logger.h")
set_target_properties(test_logger PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/logger/"
)
target_link_libraries(test_logger ${THREADS_LINK})