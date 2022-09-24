#ifndef CORE_SYSTEM_WINDOWS_SYSTEM_WINDOWS_H
#define CORE_SYSTEM_WINDOWS_SYSTEM_WINDOWS_H

#include "../system.h"

class SystemWindows final : public System {
public:
    SystemWindows()
    {
        SystemLocator::provide(this);
    }
    virtual ~SystemWindows() {}

    virtual void print(const std::string &message);
    virtual void message_box(const std::string &message,
                             const std::string &title);
};

#endif /* CORE_SYSTEM_WINDOWS_SYSTEM_WINDOWS_H */
