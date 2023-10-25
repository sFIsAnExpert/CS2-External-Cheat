#include "UI.h"

DWORD __stdcall HackThread(LPVOID Reserved) {
    while (true) {
        if (cyrix.bAimbot)
            _aim.StandardAim();
        if(cyrix.bRageAim)
            _aim.RageAim();
    }
    return 1;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    //AllocConsole(); FILE* fs;
    //freopen_s(&fs, "CONOUT$", "w", stdout);
    if (!ext.init(Xor("Counter-Strike 2"))) {
        MessageBoxA(0, Xor("CS2 Not Running!"), Xor("Failed!"), 0);
        return 0;
    }
    engine.InitEntry();
    CreateThread(0, 0, HackThread, 0, 0, 0);

    UI::SetTargetWindow(ext.ProcessInfo.hWnd);
    UI::Render();

    return 0;
}
