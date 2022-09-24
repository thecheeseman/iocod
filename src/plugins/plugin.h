#pragma once
#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include "types.h"
#include "misc.h"
#include <string>
#include <sstream>

namespace iocod {

/**
 * Helper structure for Plugin API version.
 */
struct plugin_api_version {
    static constexpr i32 major = 1;
    static constexpr i32 minor = 0;
    static constexpr i32 patch = 0;
    static constexpr i32 version = (major * 1000000) + (minor * 1000) + patch;

    static inline std::string string()
    {
        std::ostringstream buffer;
        buffer << major << '.' << minor << '.' << patch;
        return buffer.str();
    }

    static inline std::string version_to_string(int version)
    {
        std::ostringstream buffer;
        buffer << ((version) / 1000000) << '.' << ((version % 1000000) / 1000)
            << '.' << (version % 1000);
        return buffer.str();
    }

    static inline i32 decode_major(int version)
    {
        return (version / 1000000);
    }

    static inline i32 decode_minor(int version)
    {
        return ((version % 1000000) / 1000);
    }

    static inline i32 decode_patch(int version)
    {
        return (version % 1000);
    }
};

/**
 * PluginInfo structure containing all the important information about your
 * plugin. This will initialize to a blank slate, and you should use the
 * #setInfo and #setVersion functions to set the information in your
 * PluginClass constructor.
 */
struct plugin_info {
    std::string name;
    std::string author;
    std::string url;
    std::string description;

    /**
     * Set plugin information (name, author, url, description).
     */
    void set_info(const std::string& name_, const std::string& author_,
                  const std::string& url_, const std::string& description_)
    {
        name = name_;
        author = author_;
        url = url_;
        description = description_;
    }

    i32 version;
    i32 version_major;
    i32 version_minor;
    i32 version_patch;

    /**
     * Set plugin version. Recommended to use this rather than directly set
     * the members.
     */
    void set_version(i32 major, i32 minor, i32 patch)
    {
        version_major = major;
        version_minor = minor;
        version_patch = patch;
        version = major * 10000 + minor * 100 + patch;
    }

    /*
        api version
        the API version of the plugin determines if it can be loaded
        older APIs (major versions) are not compatible with newer APIs
        but patch/minor APIs changes are OK
    */
    i32 api_version = plugin_api_version::version;
};

/**
 * PluginInteface. All plugins should derive from this class.
 *
 * Plugin class constructors should not do any work other than set
 * the PluginInfo structure data. Use the #startup and #shutdown methods
 * to actually do work.
 */
class plugin {
public:
    virtual ~plugin() {}

protected:
    plugin_info info;
public:
    plugin_info get_info() const
    {
        return info;
    }

    /**
     *
     */
    virtual void startup() {}
    /**
     *
     */
    virtual void shutdown() {}
};

using plugin_funcptr_initialize = plugin * (*)();
using plugin_funcptr_implemented = void (*)();

/**
 * @def IMPLEMENT_PLUGIN
 * Must be used *once* in your plugin implementation files. This will insert the
 * necessary functions for interfacing with the system and allow the plugin
 * system to load it.
 *
 * @note In the future we can use this macro to override the default new/delete
 * operators to control how memory is allocated inside of external plugins.
 *
 * @param plugin_class  name of your plugin class which inherits from #PluginInterface
 * @param name          name of your plugin (must match the dll name EXACTLY)
 */
#define IMPLEMENT_PLUGIN(plugin_class, name) \
    extern "C" IOCOD_EXPORT iocod::plugin* IMPLEMENT_PLUGIN_initialize() \
    { \
        return new plugin_class(); \
    } \
    extern "C" IOCOD_EXPORT void IMPLEMENT_PLUGIN_##name() {} 

// namespace iocod
};

#endif /* PLUGIN_INTERFACE_H */
