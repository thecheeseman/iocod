#include "system.h"
#include <utility>
#include <cstdlib>

namespace iocod::system {

void error(const std::string& message)
{
    std::exit(1);
}

std::error_code get_last_error()
{
    return { std::error_code(errno, std::system_category()) };
}

// namespace iocod::system
};
