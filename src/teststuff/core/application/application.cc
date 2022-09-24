#include "application.h"

namespace iocod {
Application *Application::instance_ = nullptr;

Application::~Application()
{
    // shutdown
}

void Application::run()
{
    // do stuff
}
}
