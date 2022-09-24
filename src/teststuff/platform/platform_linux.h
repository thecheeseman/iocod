#include "platform.h"

class PlatformLinux final : public Platform {
public:
    ~PlatformLinux() {}

    virtual void pre_init() {}
    virtual void init() {}
    virtual void shutdown() {}

    virtual void set_crash_handler() {}

    virtual bool is_64bit() { return true; }

    virtual std::string get_environment_variable(const std::string& name) 
    {
        return { "yes" };
    }
    virtual void set_environment_variable(const std::string& name,
                                          const std::string& value) {}

    virtual PlatformVersion get_os_version();
    virtual std::string get_os_version_string();
};
