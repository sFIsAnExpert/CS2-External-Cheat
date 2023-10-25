#include "Memory.h"

Memory::External ext;

Memory::Internal in;

uintptr_t Memory::External::GetModuleBase(const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->ProcessInfo.PID);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32W modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32FirstW(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

uintptr_t Memory::External::FinalAddress(uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(this->ProcessInfo.hHand, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

bool Memory::External::init(const char* WindowName) {
    this->ProcessInfo.hWnd = FindWindowA(0, WindowName);
    if (!this->ProcessInfo.hWnd)
        return false;
    GetWindowThreadProcessId(this->ProcessInfo.hWnd, &this->ProcessInfo.PID);
    if (this->ProcessInfo.PID == 0)
        return false;
    this->ProcessInfo.hHand = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, this->ProcessInfo.PID);
    if (!this->ProcessInfo.hHand)
        return false;
    return true;
}

bool Memory::Internal::init(const char* WindowName) {
    this->ProcessInfo.hWnd = FindWindowA(0, WindowName);
    if (!this->ProcessInfo.hWnd)
        return false;
    GetWindowThreadProcessId(this->ProcessInfo.hWnd, &this->ProcessInfo.PID);
    if (this->ProcessInfo.PID == 0)
        return false;
    this->ProcessInfo.hHand = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, this->ProcessInfo.PID);
    if (!this->ProcessInfo.hHand)
        return false;
    return true;
}