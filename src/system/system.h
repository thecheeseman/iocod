#pragma once

#include <string>
#include <system_error>

namespace iocod::system {

void error(const std::string& message);
std::error_code get_last_error();

// namespace iocod::system
};
