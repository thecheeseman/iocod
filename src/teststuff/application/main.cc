#include "application.h"
#include "iocod/system.h"
#include "platform/platform.h"

#include <iostream>

int main(int argc, char *argv[])
{   
    std::cout << "Running on: " << platform->get_os_version_string() << "\n";

    // failed to initialize backend or system
    if (!app->init())
        return 1;

    // shouldn't really happen
    if (!app->init_gui())
        return 2;

    //
    // main loop
    // some internal systems can call app->request_exit()
    // which will make app->is_active() return false
    //
    while (app->is_active()) {
        // check for messages or other system resources
        if (!app->frame_begin())
            break;

        // gui building
        if (!app->frame_gui())
            break;

        // actual render
        if (!app->frame_render())
            break;

        // post adjustments
        if (!app->frame_end())
            break;
    }

    app->shutdown();
    return 0;
}
