#include "system_windows.h"
#include <iostream>

#include <windows.h>

// 
SystemWindows system_local;
System *sys = &system_local;
//

void SystemWindows::print(const std::string &message)
{
    std::cout << message << std::endl;
}

void SystemWindows::message_box(const std::string &message,
                                const std::string &title)
{
    MessageBox(nullptr, message.c_str(), title.c_str(), MB_OK);
}
