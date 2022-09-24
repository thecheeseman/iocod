#ifndef SYSTEM_WINDOWS_SYSTEM_WINDOWS_H
#define SYSTEM_WINDOWS_SYSTEM_WINDOWS_H

#include "iocod/system.h"

class SystemWindows final : public System {
public:
    ~SystemWindows() {}

    virtual void alert(const std::string &message,
                       const std::string &title) override;

    virtual const char *name() override
    {
        return "Windows";
    }
};

#endif /* SYSTEM_WINDOWS_SYSTEM_WINDOWS_H */
