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
#endif

#define PLUGIN_API_VERSION_MAJOR 1
#define PLUGIN_API_VERSION_MINOR 0
#define PLUGIN_API_VERSION_PATCH 0
#define PLUGIN_API_VERSION                                                   \
    IOCOD_VERSION_ENCODE(PLUGIN_API_VERSION_MAJOR, PLUGIN_API_VERSION_MINOR, \
                         PLUGIN_API_VERSION_PATCH)

/// @brief Plugin interface for iocod plugins.
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

typedef struct {
    uint32_t version;
    const char* name;
    const char* author;
    const char* description;
} PluginInfo;

enum {
    PLUGIN_OK,
    PLUGIN_FAILED
};

typedef void (*GetInterfacePtr)(PluginInterface* plugin_interface);
typedef PluginInfo* (*PluginInitializePtr)(GetInterfacePtr get_interface);
typedef int (*PluginShutdownPtr)(void);

#ifdef __cplusplus
#define PLUGIN_EXPORT extern "C" IOCOD_EXPORT
#else
#define PLUGIN_EXPORT IOCOD_EXPORT
#endif

#ifdef __cplusplus
}
#endif

#endif // CORE_PLUGIN_H
