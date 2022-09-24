#include "system_windows.h"
#include <windows.h>

SystemWindows local;
System *sys = &local;

void SystemWindows::alert(const std::string &message,
                          const std::string &title)
{
    MessageBoxA(nullptr, message.c_str(), title.c_str(), MB_OK);
}
