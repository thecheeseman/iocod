#include "plugins/plugin.h"

#include <iostream>

class old_api_plugin : public iocod::plugin {
public:
    old_api_plugin() 
    {
        info.set_info("old_api_plugin", "cheese", "", "a test plugin with the wrong API version");
        info.set_version(1, 0, 0);

        // hack
        info.api_version = 0;
        // hack
    }
    ~old_api_plugin() {}

    void startup() override
    {
        std::cout << "old_api_plugin startup" << std::endl;
    }

    void shutdown() override
    {
        std::cout << "old_api_plugin shutdown" << std::endl;
    }

    void coolTest()
    {
        std::cout << "Hello there\n";
    }
};

IMPLEMENT_PLUGIN(old_api_plugin, old_api_plugin);
