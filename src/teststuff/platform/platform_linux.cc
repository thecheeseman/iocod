#include "platform_linux.h"

#include <sys/utsname.h>
#include <string_view>
#include <charconv>

//
std::unique_ptr<Platform> platform = std::make_unique<PlatformLinux>();
//

PlatformVersion PlatformLinux::get_os_version()
{
    PlatformVersion version;
    version.label = "Linux";

    struct utsname sysinfo {};
    if (uname(&sysinfo) == 0) {
        const std::string_view r = sysinfo.release;

        int32_t major = 0;
        int32_t minor = 0;
        int32_t build = 0;

        const std::from_chars_result res1 =
            std::from_chars(r.data(), r.data() + r.size(), major);
        const std::from_chars_result res2 =
            std::from_chars(res1.ptr + 1, r.data() + r.size(), minor);
        const std::from_chars_result res3 =
            std::from_chars(res2.ptr + 1, r.data() + r.size(), build);
        
        if (res1.ec == std::errc {})
            version.major = major;
        if (res2.ec == std::errc {})
            version.minor = minor;
        if (res3.ec == std::errc {})
            version.build = build;

        version.sublabel = sysinfo.version;
    }
    
    return version;
}

std::string PlatformLinux::get_os_version_string()
{
    PlatformVersion version = get_os_version();
    return {
        version.label + " " + version.sublabel + " [" +
        std::to_string(version.major) + "." +
        std::to_string(version.minor) + "." +
        std::to_string(version.build) + "]"
    };
}
