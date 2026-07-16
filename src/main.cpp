#define UNICODE
#define _UNICODE
#include <windows.h>
#include "Application.h"
#include "resource.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "graphics/TextureLoader.h"
#include <d3d11.h>
#include <filesystem>
#include <vector>


#pragma comment(lib, "d3d11.lib")


static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;



bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();

void CreateRenderTarget();
void CleanupRenderTarget();


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);



std::filesystem::path GetExecutableDirectory()
{
    wchar_t modulePath[MAX_PATH] = {};
    if (GetModuleFileNameW(nullptr, modulePath, MAX_PATH) == 0)
    {
        return {};
    }

    return std::filesystem::path(modulePath).parent_path();
}

std::filesystem::path GetProjectRoot()
{
    return std::filesystem::path(__FILE__).parent_path().parent_path();
}

std::filesystem::path ResolveAssetPath(const std::filesystem::path& filename)
{
    const std::filesystem::path executableDir = GetExecutableDirectory();
    const std::filesystem::path projectRoot = GetProjectRoot();

    const std::vector<std::filesystem::path> candidates = {
        std::filesystem::current_path() / "assets" / filename,
        std::filesystem::current_path() / filename,
        executableDir / "assets" / filename,
        executableDir / filename,
        executableDir.parent_path() / "assets" / filename,
        executableDir.parent_path() / filename,
        projectRoot / "assets" / filename,
        projectRoot / "cmake" / "assets" / filename,
        projectRoot / filename
    };

    for (const auto& candidate : candidates)
    {
        if (std::filesystem::exists(candidate))
        {
            return candidate;
        }
    }

    return {};
}

std::filesystem::path ResolveLogoPath()
{
    return ResolveAssetPath(L"logopasteur.png");
}

LRESULT WINAPI WndProc(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{

    if(ImGui_ImplWin32_WndProcHandler(
        hWnd,
        msg,
        wParam,
        lParam))
    {
        return true;
    }


    switch(msg)
    {

    case WM_SIZE:

        if(g_pd3dDevice != nullptr &&
           wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();

            g_pSwapChain->ResizeBuffers(
                0,
                LOWORD(lParam),
                HIWORD(lParam),
                DXGI_FORMAT_UNKNOWN,
                0);

            CreateRenderTarget();
        }

        return 0;


    case WM_DESTROY:

        PostQuitMessage(0);

        return 0;

    }


    return DefWindowProc(
        hWnd,
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
    const wchar_t* CLASS_NAME = L"SysInfoWindowClass";
    const wchar_t* APP_TITLE = L"Info-SupportHCC";

    // Création fenêtre Windows

    HICON appIcon = static_cast<HICON>(LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_APP_ICON)));

    WNDCLASSEXW wc =
    {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        hInstance,
        appIcon,
        nullptr,
        nullptr,
        nullptr,
        CLASS_NAME,
        appIcon
    };


    RegisterClassExW(&wc);



    HWND hwnd = CreateWindowW(
        CLASS_NAME,
        APP_TITLE,
        WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME),
        100,
        100,
        700,
        800,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr);



    SetWindowTextW(hwnd, L"HCC-Info Support");

    if (appIcon != nullptr)
    {
        SendMessageW(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(appIcon));
        SendMessageW(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(appIcon));
    }

    // Initialisation DirectX

    if(!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        return 1;
    }



    ShowWindow(
        hwnd,
        SW_SHOWDEFAULT);

    UpdateWindow(hwnd);




    // Initialisation ImGui

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();


    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;



    ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.ChildRounding = 5.0f;

    style.WindowPadding = ImVec2(20,20);
    style.ItemSpacing   = ImVec2(8,10);
    style.FramePadding  = ImVec2(8,5);

    // Couleurs
    style.Colors[ImGuiCol_WindowBg]     = ImVec4(1.0f,1.0f,1.0f,1.0f);
    style.Colors[ImGuiCol_Text]         = ImVec4(0.05f,0.05f,0.05f,1.0f);
    style.Colors[ImGuiCol_Separator]    = ImVec4(0.75f,0.75f,0.75f,1.0f);
    style.Colors[ImGuiCol_Button]       = ImVec4(0.20f,0.45f,0.85f,1.0f);
    style.Colors[ImGuiCol_ButtonHovered]= ImVec4(0.30f,0.55f,0.95f,1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f,0.35f,0.70f,1.0f);


    ImGui_ImplWin32_Init(hwnd);


    ImGui_ImplDX11_Init(
    g_pd3dDevice,
    g_pd3dDeviceContext);

    TextureLoader::Initialize(g_pd3dDevice);

    Application app;

    const std::filesystem::path logoPath = ResolveLogoPath();
    if (!logoPath.empty())
    {
        const Texture logoTexture = TextureLoader::LoadLogo(logoPath.string());
        app.SetLogoTexture(logoTexture.id, logoTexture.width, logoTexture.height);
    }

    // Chargement informations système

    app.LoadInfos();




    // Boucle graphique

    MSG msg{};


    while(msg.message != WM_QUIT)
    {


        if(PeekMessage(
            &msg,
            nullptr,
            0U,
            0U,
            PM_REMOVE))
        {

            TranslateMessage(&msg);

            DispatchMessage(&msg);

            continue;
        }



        ImGui_ImplDX11_NewFrame();

        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();



        // Affichage GUI

        app.Render();



        ImGui::Render();



        float clearColor[4] =
        {
            0.94f,
            0.94f,
            0.94f,
            1.00f
        };



        g_pd3dDeviceContext->OMSetRenderTargets(
            1,
            &g_mainRenderTargetView,
            nullptr);



        g_pd3dDeviceContext->ClearRenderTargetView(
            g_mainRenderTargetView,
            clearColor);



        ImGui_ImplDX11_RenderDrawData(
            ImGui::GetDrawData());



        g_pSwapChain->Present(
            1,
            0);

    }




    // Fermeture propre

    ImGui_ImplDX11_Shutdown();

    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();



    CleanupDeviceD3D();

    DestroyWindow(hwnd);

    UnregisterClassW(
        wc.lpszClassName,
        wc.hInstance);


    return 0;
}







bool CreateDeviceD3D(HWND hWnd)
{

    DXGI_SWAP_CHAIN_DESC sd{};


    sd.BufferCount = 2;

    sd.BufferDesc.Format =
        DXGI_FORMAT_R8G8B8A8_UNORM;


    sd.BufferUsage =
        DXGI_USAGE_RENDER_TARGET_OUTPUT;


    sd.OutputWindow = hWnd;


    sd.SampleDesc.Count = 1;


    sd.Windowed = TRUE;


    sd.SwapEffect =
        DXGI_SWAP_EFFECT_DISCARD;



    D3D_FEATURE_LEVEL level;



    if(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &g_pSwapChain,
        &g_pd3dDevice,
        &level,
        &g_pd3dDeviceContext)
        != S_OK)
    {
        return false;
    }



    CreateRenderTarget();


    return true;
}




void CreateRenderTarget()
{

    ID3D11Texture2D* buffer = nullptr;


    g_pSwapChain->GetBuffer(
        0,
        IID_PPV_ARGS(&buffer));


    g_pd3dDevice->CreateRenderTargetView(
        buffer,
        nullptr,
        &g_mainRenderTargetView);



    buffer->Release();

}




void CleanupRenderTarget()
{

    if(g_mainRenderTargetView)
    {

        g_mainRenderTargetView->Release();

        g_mainRenderTargetView = nullptr;

    }

}




void CleanupDeviceD3D()
{

    CleanupRenderTarget();


    if(g_pSwapChain)
        g_pSwapChain->Release();


    if(g_pd3dDeviceContext)
        g_pd3dDeviceContext->Release();


    if(g_pd3dDevice)
        g_pd3dDevice->Release();

}