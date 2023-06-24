// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_PLUGIN_H
#define CORE_PLUGIN_H

#include <Core/Platform.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace iocod {
#endif

#define PLUGIN_API_VERSION_MAJOR 1
#define PLUGIN_API_VERSION_MINOR 0
#define PLUGIN_API_VERSION_PATCH 0
#define PLUGIN_API_VERSION                                                   \
    IOCOD_VERSION_ENCODE(PLUGIN_API_VERSION_MAJOR, PLUGIN_API_VERSION_MINOR, \
                         PLUGIN_API_VERSION_PATCH)

/// @brief Plugin interface for iocod plugins.
///
/// @details This structure should be defined locally in the plugin and passed
/// to the get_interface function pointer during PluginInitialize. The api and
/// size fields must be set by the plugin prior to calling get_interface. The
/// process looks something like this:
///
/// @code
/// PluginInterface interface;
/// PLUGIN_EXPORT PluginInfo* PluginInitialize(GetInterfacePtr get_interface)
/// {
///     interface.api = PLUGIN_API_VERSION;
///     interface.size = sizeof(PluginInterface);
///     get_interface(&interface);
///     ...
/// }
/// @endcode
///
/// By setting api and size, the plugin is telling iocod what version of the
/// plugin requires. The system can then fill in the rest of the API fields
/// with the correct function pointers.
///
/// The example code is recommended if you just want to use the latest API
/// whenever your plugin is compiled. If you need a specific version, you can
/// always encode that with the IOCOD_VERSION_ENCODE macro and the specific
/// size of the PluginInterface struct at that time.
typedef struct {
    //
    // defined by plugin before initialization
    //

    // both of these fields must be set by the plugin prior to calling
    // the get_interface function ptr passed to PluginInitialize
    uint32_t api;
    size_t size;

    //
    // defined by iocod after plugin initialization
    //

    void (*log)(const char* message);
} PluginInterface;

/// @brief Plugin information structure.
typedef struct {
    uint32_t version;        ///< Plugin version
    const char* name;        ///< Long name of plugin
    const char* author;      ///< Author
    const char* description; ///< Preferably a single-line description
} PluginInfo;

/// @brief Function pointer for getting plugin API interface.
typedef void (*GetInterfacePtr)(PluginInterface* plugin_interface);

/// @brief Plugin initialization function pointer.
typedef PluginInfo* (*PluginInitializePtr)(GetInterfacePtr get_interface);

/// @brief Plugin shutdown function pointer.
typedef void (*PluginShutdownPtr)(void);

/// @def PLUGIN_EXPORT
/// @brief Attribute to make exported plugin functions visible to the linker.
#ifdef __cplusplus
    #define PLUGIN_EXPORT extern "C" IOCOD_EXPORT
#else
    #define PLUGIN_EXPORT IOCOD_EXPORT
#endif

#ifdef __cplusplus
}
} // namespace iocod
#endif

#endif // CORE_PLUGIN_H
