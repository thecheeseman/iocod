#include "system_linux.h"
#include <iostream>

// 
SystemLinux system_local;
System *sys = &system_local;
//

void SystemLinux::print(const std::string &message)
{
    std::cout << message << std::endl;
}

void SystemLinux::message_box(const std::string &message,
                                const std::string &title)
{
    std::cout << message << std::endl;
}
