#ifndef CORE_SYSTEM_LINUX_SYSTEM_LINUX_H
#define CORE_SYSTEM_LINUX_SYSTEM_LINUX_H

#include "../system.h"

class SystemLinux final : public System {
public:
    virtual ~SystemLinux() {}

    virtual void print(const std::string &message);
    virtual void message_box(const std::string &message,
                             const std::string &title);
};

#endif /* CORE_SYSTEM_LINUX_SYSTEM_LINUX_H */
