#include "application.h"

class ApplicationLinux final : public Application {
public:
    virtual ~ApplicationLinux() {}

    virtual bool init() override;
    virtual bool init_gui() override;
    virtual bool frame_begin() override;
    virtual bool frame_gui() override;
    virtual bool frame_render() override;
    virtual bool frame_end() override;
    virtual void shutdown() override;
};

ApplicationLinux localApp;
Application *app = &localApp;

bool ApplicationLinux::init()
{
    return true;
}

bool ApplicationLinux::init_gui()
{
    return true;
}

bool ApplicationLinux::frame_begin()
{
    return true;
}

bool ApplicationLinux::frame_gui()
{
    return true;
}

bool ApplicationLinux::frame_render()
{
    return true;
}

bool ApplicationLinux::frame_end()
{
    return true;
}

void ApplicationLinux::shutdown()
{

}
