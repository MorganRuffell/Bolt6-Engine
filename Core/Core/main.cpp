#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "resource.h"
#include <iostream>
#include <d3d11.h>
#include <tchar.h>
#include <Core/Core/Engine/TestEngine.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

TestEngine Engine;


int main(int, char**)
{
    std::cout << "Bolt6.ai Programming Test" << std::endl;
    std::cout << "Candidate: Mr Morgan Ruffell" << std::endl;
    std::cout << "Task Set By: Mr Luke Dodd" << std::endl << std::endl;

    CoInitialize(NULL);

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX GraphicsWindow = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Bolt6 Programming Test - DX11"), NULL };

    GraphicsWindow.hIconSm = LoadIcon(GraphicsWindow.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    GraphicsWindow.hIcon = LoadIcon(GraphicsWindow.hInstance, MAKEINTRESOURCE(IDI_ICON1));


    ::RegisterClassEx(&GraphicsWindow);
    HWND hwnd = ::CreateWindow(GraphicsWindow.lpszClassName, _T("Bolt6 Programming Test - DX11"), WS_OVERLAPPEDWINDOW, 100, 100, 2560, 1440, NULL, NULL, GraphicsWindow.hInstance, NULL);


    Engine.Init(hwnd);
    bool Complete = false;
    bool Paused = false;

    std::cout << "Engine Initailzation Complete" << std::endl;

    while (!Complete)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                Complete = true;

            if (msg.message == WM_ACTIVATE)
                Paused = true;
        }
        if (Complete)
            break;


        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Engine.Update();


        Engine.Render();
   
    }

    std::cout << "Engine Shutdown Complete!" << std::endl;

    Engine.Terminate();


    return 0;
}


// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_ACTIVATE:
        return 0;
    case WM_SIZE:
        if (Engine.m_GraphicsDevice->m_Accelerator->Device != NULL && wParam != SIZE_MINIMIZED)
        {
            Engine.m_GraphicsDevice->CleanupRenderTarget();
            Engine.m_GraphicsDevice->GetSwapChain()->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            Engine.m_GraphicsDevice->CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
