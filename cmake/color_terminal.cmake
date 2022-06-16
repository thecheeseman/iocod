# colorize CMake output
# code adapted from stackoverflow: http://stackoverflow.com/a/19578320
# from post authored by https://stackoverflow.com/users/2556117/fraser
macro(setup_color_terminal)
    set(SUPPORT_ASCII_COLORS OFF)

    message(CHECK_START "Checking if terminal supports ASCII color")
    if (NOT WIN32)
        string(FIND ENV{TERM} "color" COLOR_SHELL)

        # this isn't foolproof but it's worth a try
        if (NOT ${COLOR_SHELL} EQUAL -1)
            set(SUPPORT_ASCII_COLORS ON)
        endif()
    endif()

    option(ENABLE_ASCII_COLORS "Enable ASCII/VTI color processing" OFF)
    mark_as_advanced(ENABLE_ASCII_COLORS)
    option(FORCE_DISABLE_ASCII_COLORS "Force disable ASCII/VTI color processing" OFF)
    mark_as_advanced(FORCE_DISABLE_ASCII_COLORS)

    # force disable 
    if (FORCE_DISABLE_ASCII_COLORS)
        set(SUPPORT_ASCII_COLORS OFF)
        set(ENABLE_ASCII_COLORS OFF)
    endif()

    # if not force disabling, we want to force enable them on
    if (ENABLE_ASCII_COLORS)
        set(SUPPORT_ASCII_COLORS ON)
    endif()

    if (SUPPORT_ASCII_COLORS)
        if (ENABLE_ASCII_COLORS)
            message(CHECK_PASS "enabled (forced)")
        else()
            message(CHECK_PASS "enabled")
        endif()

        string(ASCII 27 Esc)
        set(ColorReset "${Esc}[m")
        set(ColorBold "${Esc}[1m")
        set(Red "${Esc}[31m")
        set(Green "${Esc}[32m")
        set(Yellow "${Esc}[33m")
        set(Blue "${Esc}[34m")
        set(Magenta "${Esc}[35m")
        set(Cyan "${Esc}[36m")
        set(White "${Esc}[37m")
        set(BoldRed "${Esc}[1;31m")
        set(BoldGreen "${Esc}[1;32m")
        set(BoldYellow "${Esc}[1;33m")
        set(BoldBlue "${Esc}[1;34m")
        set(BoldMagenta "${Esc}[1;35m")
        set(BoldCyan "${Esc}[1;36m")
        set(BoldWhite "${Esc}[1;37m")
    else()
        if (FORCE_DISABLE_ASCII_COLORS)
            message(CHECK_FAIL "disabled (forced)")
        else()
            message(CHECK_FAIL "disabled")
        endif()

        set(ColorReset "")
        set(ColorBold "")
        set(Red "")
        set(Green "")
        set(Yellow "")
        set(Blue "")
        set(Magenta "")
        set(Cyan "")
        set(White "")
        set(BoldRed "")
        set(BoldGreen "")
        set(BoldYellow "")
        set(BoldBlue "")
        set(BoldMagenta "")
        set(BoldCyan "")
        set(BoldWhite "")
    endif()
endmacro()
