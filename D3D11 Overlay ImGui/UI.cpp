#include "UI.h"
#include "Drawing.h"

ID3D11Device* UI::pd3dDevice = nullptr;
ID3D11DeviceContext* UI::pd3dDeviceContext = nullptr;
IDXGISwapChain* UI::pSwapChain = nullptr;
ID3D11RenderTargetView* UI::pMainRenderTargetView = nullptr;
bool UI::bInit = false;
HWND UI::hTargetWindow = nullptr;
BOOL UI::bTargetSet = FALSE;
DWORD UI::dTargetPID = 0;

HMODULE UI::hCurrentModule = nullptr;

bool UI::CreateDeviceD3D(HWND hWnd)
{
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

    const UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void UI::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != nullptr)
    {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}

void UI::CleanupRenderTarget()
{
    if (pMainRenderTargetView)
    {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}

void UI::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (pSwapChain)
    {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }

    if (pd3dDeviceContext)
    {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
    }

    if (pd3dDevice)
    {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
    {
        MARGINS     Margin = { -1 };
        DwmExtendFrameIntoClientArea(hWnd, &Margin);
        break;
    }
    case WM_SIZE:
        if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;

    default:
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

std::string random_string(std::size_t length)
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

bool UpdateWindowData()
{
    POINT Point{};
    RECT Rect{};

    GetClientRect(ext.ProcessInfo.hWnd, &Rect);
    ClientToScreen(ext.ProcessInfo.hWnd, &Point);

    UI::WindowPos = UI::DestWindowPos = Vector2(static_cast<float>(Point.x), static_cast<float>(Point.y));
    UI::WindowSize = UI::DestWindowSize = Vector2(static_cast<float>(Rect.right), static_cast<float>(Rect.bottom));

    SetWindowPos(UI::OverlayhWnd, HWND_TOPMOST, (int)UI::WindowPos.x, (int)UI::WindowPos.y, (int)UI::WindowSize.x, (int)UI::WindowSize.y, SWP_SHOWWINDOW);

    // ¿ØÖÆ´°¿Ú×´Ì¬ÇÐ»»
    POINT MousePos;
    GetCursorPos(&MousePos);
    ScreenToClient(UI::OverlayhWnd, &MousePos);
    ImGui::GetIO().MousePos.x = static_cast<float>(MousePos.x);
    ImGui::GetIO().MousePos.y = static_cast<float>(MousePos.y);

    if (ImGui::GetIO().WantCaptureMouse)
        SetWindowLong(UI::OverlayhWnd, GWL_EXSTYLE, GetWindowLong(UI::OverlayhWnd, GWL_EXSTYLE) & (~WS_EX_LAYERED));
    else
        SetWindowLong(UI::OverlayhWnd, GWL_EXSTYLE, GetWindowLong(UI::OverlayhWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    return true;
}


void UI::Render()
{
    ImGui_ImplWin32_EnableDpiAwareness();
    std::string randName = random_string(6);
    // Get the main window of the process when overlay as DLL
    #ifdef _WINDLL
    if (hTargetWindow == nullptr)
        GetWindow();
    #endif

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Cyrix Window", NULL};

    RegisterClassExW(&wc);
    const HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,L"Cyrix Window", L"Cyrix", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, NULL, NULL, GetModuleHandle(NULL), NULL);
    UI::OverlayhWnd = hwnd;
    const MARGINS margin = {-1, 0, 0, 0};
    DwmExtendFrameIntoClientArea(hwnd, &margin);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    // Scale the font size depending of the screen size.
    const HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    const int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

    if (monitor_height > 1080)
    {
        const float fScale = 2.0f;
        ImFontConfig cfg;
        ImGui::GetIO().Fonts->AddFontDefault(&cfg);
    }

    ImGui::GetIO().IniFilename = nullptr;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    const ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    bInit = true;

    bool bDone = false;

    while (!bDone)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                bDone = true;
        }

        if (!UpdateWindowData())
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            Drawing::Draw();
        }
        ImGui::EndFrame();        

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
        pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        pSwapChain->Present(1, 0);
    }

    bInit = false;

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

#ifdef _WINDLL
    CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, hCurrentModule, NULL, nullptr);
#else
    TerminateProcess(GetModuleHandleA(nullptr), 0);
#endif
}

void UI::MoveWindow(const HWND hCurrentProcessWindow)
{
    RECT rect;
    if (hTargetWindow == nullptr)
        return;

    GetWindowRect(hTargetWindow, &rect);

    int lWindowWidth = rect.right - rect.left;
    int lWindowHeight = rect.bottom - rect.top;

    lWindowWidth -= 5;
    lWindowHeight -= 29;

    SetWindowPos(hCurrentProcessWindow, nullptr, rect.left, rect.top, lWindowWidth, lWindowHeight, SWP_SHOWWINDOW);
}

BOOL UI::IsWindowValid(const HWND hCurrentWindow)
{
    DWORD styles, ex_styles;
    RECT rect;

    if (!IsWindowVisible(hCurrentWindow) ||
        (IsIconic(hCurrentWindow) || IsWindowCloaked(hCurrentWindow)))
        return FALSE;

    GetClientRect(hCurrentWindow, &rect);
    styles = (DWORD)GetWindowLongPtr(hCurrentWindow, GWL_STYLE);
    ex_styles = (DWORD)GetWindowLongPtr(hCurrentWindow, GWL_EXSTYLE);

    if (ex_styles & WS_EX_TOOLWINDOW)
        return FALSE;
    if (styles & WS_CHILD)
        return FALSE;
    if (rect.bottom == 0 || rect.right == 0)
        return FALSE;

    return TRUE;
}

BOOL UI::IsWindowCloaked(const HWND hCurrentWindow)
{
    DWORD cloaked;
    const HRESULT hr = DwmGetWindowAttribute(hCurrentWindow, DWMWA_CLOAKED, &cloaked,
        sizeof(cloaked));
    return SUCCEEDED(hr) && cloaked;
}


void UI::SetTargetWindow(const HWND hWindow)
{
    hTargetWindow = hWindow;
    SetForegroundWindow(hTargetWindow);
    GetWindowThreadProcessId(hTargetWindow, &dTargetPID);
    bTargetSet = TRUE;
}

BOOL UI::IsWindowAlive()
{
    DWORD dCurrentPID;

    if (hTargetWindow == nullptr)
        return FALSE;

    if (!IsWindow(hTargetWindow))
        return FALSE;

    GetWindowThreadProcessId(hTargetWindow, &dCurrentPID);

    if (dCurrentPID != dTargetPID)
        return FALSE;

    return TRUE;
}