# Plugin ideas

## Interface

- All plugins should inherit from `PluginInterface` class with basic 
  overrideable functionality
- Need some way of handling game hooks and stuff

UE5 does implementation via inheritance like this + with some macro which
"implements" the module
```cpp
// somewhere in one C++ source file for the module
IMPLEMENT_MODULE(ModuleClass, ModuleName)
```

This roughly compiles to
```cpp
extern "C" DLLEXPORT IModuleInterface* InitializeModule()
{
    return new ModuleClass();
}

// check to make sure all modules use the macro
extern "C" void IMPLEMENT_MODULE##ModuleName() {}
```

UE5 overrides the default `new`/`delete` operators via some boilerplate that is
inserted with the macro after the `IMPLEMENT_MODULE##ModuleName()` function.
This forces each module to run `new`/`delete` through the 
`FMemory::Malloc` and `FMemory::Free` functions 

## Format

- shared libraries (`.dll`, `.so`, `.dylib`)
- probably in the `./plugins/` path

## Configuration

Maybe plugins could use .ini or XML or other parsers for config?

## Manifests

Plugin information/dependencies could be stored in separate manifest files

### UE5 example 
```json
{
	"FileVersion": 3,
	"Version": 1,
	"VersionName": "1.8.1",
	"FriendlyName": "ProjectCleaner",
	"Description": "Unreal engine plugin for managing all unused assets and empty folders in project",
	"Category": "ProjectManagementTools",
	"CreatedBy": "Ashot Barkhudaryan",
	"CreatedByURL": "https://github.com/ashe23",
	"DocsURL": "https://github.com/ashe23/ProjectCleaner",
	"MarketplaceURL": "com.epicgames.launcher://ue/marketplace/product/4d7f5dc837fc4b009bb91e678adf9fd0",
	"SupportURL": "",
	"EngineVersion": "4.27.0",
	"CanContainContent": false,
	"Installed": true,
	"Modules": [
		{
			"Name": "ProjectCleaner",
			"Type": "Editor",
			"LoadingPhase": "PostDefault",
			"WhitelistPlatforms": [
				"Win64",
				"Win32",
				"Linux"
			]
		}
	]
}
```
