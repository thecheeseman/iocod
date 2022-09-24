#include "application.h"

// tell windows not to generate a console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_custom.h"
#include <d3d11.h>
#include <tchar.h>

class ApplicationDirectX11 final : public Application {
public:
    virtual ~ApplicationDirectX11() {}

    virtual bool init() override;
    virtual bool init_gui() override;
    virtual bool frame_begin() override;
    virtual bool frame_gui() override;
    virtual bool frame_render() override;
    virtual bool frame_end() override;
    virtual void shutdown() override;

    ImVec4 bg_color = ImVec4(0.2f, 0.3f, 0.2f, 1.00f);
    WNDCLASSEX wc;
    HWND hwnd;
    
    ID3D11Device *g_pd3dDevice = NULL;
    ID3D11DeviceContext *g_pd3dDeviceContext = NULL;
    IDXGISwapChain *g_pSwapChain = NULL;
    ID3D11RenderTargetView *g_mainRenderTargetView = NULL;

    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
};

ApplicationDirectX11 localApp;
Application *app = &localApp;

// cannot be inside class
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool ApplicationDirectX11::init()
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(NULL),
        NULL, NULL,
        NULL, NULL,
        _T("iocod"),
        NULL
    };
    ::RegisterClassEx(&wc);
    hwnd = ::CreateWindow(wc.lpszClassName,
                          _T("iocod console"),
                          WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME),
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          1024, 640,
                          NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    active = true;
    return true;
}

bool ApplicationDirectX11::init_gui()
{
    ImGui::CustomSetup();
    ImGuiIO &io = ImGui::GetIO(); (void) io;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    return true;
}

bool ApplicationDirectX11::frame_begin()
{
    // Poll and handle messages (inputs, window resize, etc.)
    // See the WndProc() function below for our to dispatch events to the Win32 backend.
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            return false;
    }

    return true;
}

bool ApplicationDirectX11::frame_gui()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::CustomFrame();

    return true;
}

bool ApplicationDirectX11::frame_render()
{
    // Rendering
    ImGui::Render();
    const float clear_color_with_alpha[4] = { 
        bg_color.x * bg_color.w, 
        bg_color.y * bg_color.w, 
        bg_color.z * bg_color.w, 
        bg_color.w 
    };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    #if 0
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    #endif

    g_pSwapChain->Present(1, 0); // Present with vsync
    //g_pSwapChain->Present(0, 0); // Present without vsync

    return true;
}

bool ApplicationDirectX11::frame_end()
{
    return true;
}

void ApplicationDirectX11::shutdown()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

bool ApplicationDirectX11::CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { 
        D3D_FEATURE_LEVEL_11_0, 
        D3D_FEATURE_LEVEL_10_0, 
    };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
                                      NULL, createDeviceFlags,
                                      featureLevelArray, 2,
                                      D3D11_SDK_VERSION, &sd,
                                      &g_pSwapChain, &g_pd3dDevice,
                                      &featureLevel,
                                      &g_pd3dDeviceContext) != S_OK) {
        return false;
    }

    CreateRenderTarget();
    return true;
}

void ApplicationDirectX11::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { 
        g_pSwapChain->Release(); 
        g_pSwapChain = NULL; 
    }
    if (g_pd3dDeviceContext) { 
        g_pd3dDeviceContext->Release(); 
        g_pd3dDeviceContext = NULL; 
    }
    if (g_pd3dDevice) { 
        g_pd3dDevice->Release(); 
        g_pd3dDevice = NULL; 
    }
}

void ApplicationDirectX11::CreateRenderTarget()
{
    ID3D11Texture2D *pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, 
                                         &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void ApplicationDirectX11::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { 
        g_mainRenderTargetView->Release(); 
        g_mainRenderTargetView = NULL; 
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, 
                                                             UINT msg, 
                                                             WPARAM wParam, 
                                                             LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (localApp.g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            localApp.CleanupRenderTarget();
            localApp.g_pSwapChain->ResizeBuffers(0, (UINT) LOWORD(lParam),
                                                 (UINT) HIWORD(lParam), 
                                                 DXGI_FORMAT_UNKNOWN, 0);
            localApp.CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT *suggested_rect = (RECT *) lParam;
            ::SetWindowPos(hWnd, NULL, suggested_rect->left, 
                           suggested_rect->top, 
                           suggested_rect->right - suggested_rect->left, 
                           suggested_rect->bottom - suggested_rect->top, 
                           SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
