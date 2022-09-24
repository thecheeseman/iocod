#pragma once

#include <string>
#include <memory>

struct PlatformVersion {
    ~PlatformVersion() {}

    int32_t major;
    int32_t minor;
    int32_t build;

    enum class ErrorCode {
        Ok,
        UnknownVersion,
        ProductInfoFailed,
        GetVersionFailed
    };
    ErrorCode error;

    // version labels
    std::string label;
    std::string sublabel;
};

class Platform {
public:
    virtual ~Platform() {}

    virtual void pre_init() = 0;
    virtual void init() = 0;
    virtual void shutdown() = 0;

    virtual void set_crash_handler() = 0;

    virtual bool is_64bit() = 0;

    virtual std::string get_environment_variable(const std::string& name) = 0;
    virtual void set_environment_variable(const std::string& name,
                                          const std::string& value) = 0;

    virtual PlatformVersion get_os_version() = 0;
    virtual std::string get_os_version_string() = 0;
};

extern std::unique_ptr<Platform> platform;
