#include <Windows.h>

#include <d3d11.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Application.h"


static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;


extern LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);


LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
)
{
    if(ImGui_ImplWin32_WndProcHandler(
        hwnd,
        msg,
        wParam,
        lParam))
        return true;


    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }


    return DefWindowProc(
        hwnd,
        msg,
        wParam,
        lParam);
}


int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int)
{

    Application app;

    app.LoadInfos();


    // Ici création fenêtre + DirectX11


    ImGui::CreateContext();


    ImGui_ImplWin32_Init(hwnd);

    ImGui_ImplDX11_Init(
        g_pd3dDevice,
        g_pd3dContext
    );


    MSG msg{};


    while(msg.message != WM_QUIT)
    {

        if(PeekMessage(
            &msg,
            nullptr,
            0,
            0,
            PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }


        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();


        app.Render();


        ImGui::Render();


        float clear_color[] =
        {
            0.1f,
            0.1f,
            0.1f,
            1.0f
        };


        g_pd3dContext->OMSetRenderTargets(
            1,
            &g_mainRenderTargetView,
            nullptr
        );


        g_pd3dContext->ClearRenderTargetView(
            g_mainRenderTargetView,
            clear_color
        );


        ImGui_ImplDX11_RenderDrawData(
            ImGui::GetDrawData()
        );


        g_pSwapChain->Present(
            1,
            0
        );
    }


    return 0;
}