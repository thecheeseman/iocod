# engine ideas

## our process?

```cpp
void engine::initialize()
{
    // handle setup of critical sections
    // main thread stuff
    // cpu/memory/video card detection stuff
    // sse2 etc.
    // sys_millis
    // other thread stuff here
    system::initialize();

    // com_init stuff
    common::initialize();

    // added later when we do graphics stuff
    renderer::initialize();
}

void engine::run()
{
    while (true) {
        if (dedicated)
            sleep(5);

        common::frame();
    }
}

void engine::shutdown()
{
    // de init etc
}

int main()
{
    engine& iocod = engine::get();
    iocod.initialize();
    iocod.run();
    iocod.shutdown();
}
```
## cod4 process

```cpp
int main()
{
    Sys_InitializeCriticalSections();
    Sys_InitMainThread();

    Win_InitLocalization();

    if (Sys_CheckCrashOrRerun()) {
        Com_InitParse();

        Dvar_Init();

        InitTiming();

        // sys stuff
        Sys_GetCpuCount();
        Sys_CpuGHz();
        Sys_SystemMemoryMB();
        Sys_DetectVideoCard();
        Sys_SupportsSSE();
        Sys_DetectCpuVendorAndName(;
        Sys_SetAutoConfigureGHz();
        // sys stuff

        Sys_CreateSplashWindow();
        Sys_ShowSplashWindow();

        D3DXCompileShaderInit();

        Sys_Milliseconds();
        Sys_InitStreamThread();
        Com_Init();
        
        // cod4 stats
        if (com_dedicated->integer == 0) {
            Cbuf_AddText(0,"readStats\n");
        }
        
        Com_Printf(0x10,"Working directory: %s\n");

        // pb client
        if (com_dedicated->integer == 0)
            PbClientInitialize();

        PbServerInitialize();

        while (true) {
            if (com_dedicated->integer)
                _WinSleep(5);

            Com_Frame();

            if (com_dedicated->integer == 0) {
                PbClientProcessEvents();
                PbServerProcessEvents();
            }
            else {
                PbServerProcessEvents();
            }
        }
    }

    Win_ShutdownLocalization();
    return 0;
}
```
