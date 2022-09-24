#include "plugins/plugin.h"

#include <iostream>

class bad_plugin : public iocod::plugin {
public:
    bad_plugin()
    {
        info.set_info("bad_plugin", "cheese", "", "a bad plugin");
        info.set_version(1, 0, 0);
    }
    ~bad_plugin() {}

    void startup() override
    {
        std::cout << "bad_plugin startup" << std::endl;
    }

    void shutdown() override
    {
        std::cout << "bad_plugin shutdown" << std::endl;
    }
};

// IMPLEMENT_PLUGIN(bad_plugin, bad_plugin);
