#include "iocod.h"
#include <iostream>
#include <source_location>

int main(int argc, char* argv[])
{
    using namespace iocod;
    plugin_manager::instance().load_plugin_folder("plugins");
    plugin_manager::instance().load_plugin_folder("plugins");
    plugin_manager::instance().plugin_startup();
    std::cout << "hello there\n";
    plugin_manager::instance().plugin_shutdown();
}
