#include "plugin_manager.h"
#include "system/shared_library.h"
#include <optional>
#include <iostream>
#include <algorithm>

namespace iocod {

plugin_manager::plugin_manager()
{

}

plugin_manager::~plugin_manager()
{
    plugin_shutdown();
}

plugin* plugin_manager::load_plugin(const std::filesystem::path& path)
{
    // we really only want to store the stem of the path
    // because we don't really want to deal with referencing loaded
    // plugins via their full path
    const std::string name = path.stem().string();
    if (plugins.contains(name)) {
        std::cout << "[plugin_manager] Tried loading plugin \"" << name << "\" twice\n";
        return nullptr;
    }

    std::cout << "[plugin_manager] Loading plugin \"" << name << "\"\n";
    plugin_data& plugin = plugins[name];
    plugin.path = path;

    if (!plugin.handle.load(path)) {
        plugin.status = plugin_error::file_not_found;
        return nullptr;
    }

    // strip filesystem path to just the stem
    const std::string implement_name = "IMPLEMENT_PLUGIN_" + name;

    // all plugins are required to have these two functions implemented
    // (auto-implemented with the IMPLEMENT_PLUGIN() macro)
    auto plugin_implemented = plugin.handle.get<plugin_funcptr_implemented>(implement_name.c_str());
    if (!plugin_implemented) {
        plugin.status = plugin_error::invalid_format;
        return nullptr;
    }

    // initialize is required to do proper new/delete operator overloads
    auto plugin_init = plugin.handle.get<plugin_funcptr_initialize>("IMPLEMENT_PLUGIN_initialize");
    if (!plugin_init) {
        plugin.status = plugin_error::invalid_format;
        return nullptr;
    }

    // try to initialize and get the plugin's information
    try {
        plugin.interface = plugin_init();
        plugin.info = { plugin_info(plugin.interface->get_info()) };
    } catch (...) {
        std::cout << "[plugin_manager] Couldn't initialize plugin \"" << name << "\"\n";
        plugin.status = plugin_error::initialization_error;
        throw;
    }

    // check info
    // currently only concerned with major versions but other stuff could be checked here
    if (plugin_api_version::decode_major(plugin.info.api_version) < plugin_api_version::major) {
        std::cout << "[plugin_manager] Plugin \"" << name << "\" has wrong API version (" <<
            plugin_api_version::version_to_string(plugin.info.api_version) << " < "
            << plugin_api_version::string() << ")\n";
        plugin.status = plugin_error::incompatible_api;
        return nullptr;
    }

    plugin.status = plugin_error::ok;
    return plugin.interface;
}

static bool is_dll(const std::filesystem::path& path)
{
    if (path.has_extension() && (path.extension() == ".dll" ||
        path.extension() == ".so" || path.extension() == ".dylib")) {
        return true;
    }

    return false;
}

void plugin_manager::load_plugin_folder(const std::filesystem::path& path)
{
    std::cout << "[plugin_manager] Loading plugin folder " << path << "\n";

    for (const auto& entry : std::filesystem::directory_iterator { path }) {
        if (entry.is_regular_file() && is_dll(entry.path())) {
            plugin_manager::instance().load_plugin(entry.path());
        }
    }
}

void plugin_manager::plugin_startup()
{
    std::cout << "[plugin_manager] Starting up plugins\n";
    for (auto& [name, plugin] : plugins) {
        if (plugin.status == plugin_error::ok) {
            plugin.interface->startup();
        }
    }
}

void plugin_manager::plugin_shutdown()
{
    std::cout << "[plugin_manager] Shutting down plugins\n";
    for (auto& [name, plugin] : plugins) {
        if (plugin.status == plugin_error::ok) {
            plugin.interface->shutdown();
            plugin.status = plugin_error::shutdown;
        }
    }
}

// namespace iocod
}; 
