if (IOCOD_PLATFORM_MSVC)
	target_sources(iocod PRIVATE "system_windows.cxx")
else()
    target_sources(iocod PRIVATE "system_linux.cxx")
endif()

target_sources(iocod PRIVATE "shared_library.cxx")
