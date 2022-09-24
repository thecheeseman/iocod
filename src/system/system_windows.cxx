#include "System.h"
#include <utility>
#include <cstdlib>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace iocod::system {

void error(const std::string& message)
{
    MessageBoxA(nullptr, message.c_str(), "Error", MB_OK | MB_ICONERROR);
    std::exit(1);
}

std::error_code get_last_error()
{
    return { std::error_code(GetLastError(), std::system_category()) };
}

// namespace iocod::system
};
