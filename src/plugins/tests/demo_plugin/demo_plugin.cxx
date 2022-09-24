#include "plugins/plugin.h"

#include <iostream>

class demo_plugin : public iocod::plugin {
public:
    demo_plugin()
    {
        info.set_info("demo_plugin", "cheese", "", "a demo plugin");
        info.set_version(1, 0, 0);
    }
    ~demo_plugin() {}

    void startup() override
    {
        std::cout << "demo_plugin startup" << std::endl;
    }

    void shutdown() override
    {
        std::cout << "demo_plugin shutdown" << std::endl;
    }

    void coolTest()
    {
        std::cout << "Hello there\n";
    }
};

IMPLEMENT_PLUGIN(demo_plugin, demo_plugin);
