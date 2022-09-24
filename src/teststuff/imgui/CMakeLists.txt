target_sources(iocod PRIVATE
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "imgui_stdlib.cpp"
)

if (IOCOD_PLATFORM_WINDOWS)
    if (IOCOD_DX_TARGET EQUAL 09)
        target_sources(iocod PRIVATE "imgui_impl_dx9.cpp")
    elseif (IOCOD_DX_TARGET EQUAL 10)
        target_sources(iocod PRIVATE "imgui_impl_dx10.cpp")
    elseif (IOCOD_DX_TARGET EQUAL 11)
        target_sources(iocod PRIVATE "imgui_impl_dx11.cpp")
    elseif(IOCOD_DX_TARGET EQUAL 12)
        target_sources(iocod PRIVATE "imgui_impl_dx12.cpp")
    endif()

    target_sources(iocod PRIVATE "imgui_impl_win32.cpp")
else()

endif()
