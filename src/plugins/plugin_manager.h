#pragma once
#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "types.h"
#include "misc.h"

#include "plugin.h"
#include "system/shared_library.h"

#include "singleton.h"

#include <filesystem>
#include <map>
#include <string>
#include <memory>

namespace iocod {

enum class plugin_error {
    ok,
    file_not_found,
    invalid_format,
    initialization_error,
    incompatible_api,
    unloaded,
    shutdown // only occurs when the plugin manager is shutting down
};

class plugin_manager : public singleton<plugin_manager> {
    CLASS_SINGLETON(plugin_manager);

    class plugin_data {
    public:
        std::filesystem::path path;
        plugin_error status = plugin_error::unloaded;
        iocod::system::shared_library handle;
        plugin* interface;
        plugin_info info;

        i32 load_order = 0;
        static inline i32 current_load_order = 0;

        plugin_data() :
            load_order(current_load_order++)
        {
        }
        ~plugin_data() {}
    };

    std::map<std::string, plugin_data> plugins;

public:
    static inline void shutdown() {};

    /**
     * @brief Load a given plugin via a direct path.
     * @param[in] path the path to the plugin
     * @return a pointer to the plugin interface, or nullptr if failed
     */
    plugin* load_plugin(const std::filesystem::path& path);

    /**
     * @brief Load any valid shared libraries in a given folder path non-recursively.
     * @param[in] path the path to the folder
     */
    void load_plugin_folder(const std::filesystem::path& path);

    void plugin_startup();
    void plugin_shutdown();
};

// namespace iocod
};

#endif /* PLUGIN_MANAGER_H */
