#pragma once
#include "../../pch.h"

namespace Memory {
    class ProcessInfo {
    public:
        HANDLE hHand = nullptr;
        HWND hWnd = nullptr;
        DWORD PID = 0;
    };

    class External {
    public:
        ProcessInfo ProcessInfo;
        bool init(const char*);
        uintptr_t GetModuleBase(const wchar_t* modName);
        uintptr_t FinalAddress(uintptr_t ptr, std::vector<unsigned int> offsets);
        template<typename T> T RPM(uintptr_t address) {
            T buffer;
            ReadProcessMemory(ProcessInfo.hHand, (LPVOID)address, &buffer, sizeof(buffer), nullptr);

            return buffer;
        }
        template<typename T> T WPM(uintptr_t address, T val) {
            T buffer = (T)val;
            WriteProcessMemory(ProcessInfo.hHand, (LPVOID)address, &buffer, sizeof(buffer), nullptr);

            return buffer;
        }
    };
    class Internal {
    public:
        ProcessInfo ProcessInfo;
        bool init(const char*);
        static bool DataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
        {
            for (; *szMask; ++szMask, ++pData, ++bMask)
            {
                if (*pData != *bMask && *szMask == 'x')
                    return false;
            }

            return (*szMask) == NULL;
        }
        static DWORD FindPattern(DWORD dwAddress, DWORD dwSize, BYTE* pbMask, const char* szMask)
        {
            for (DWORD i = 0; i < dwSize; i++)
            {
                if (DataCompare(reinterpret_cast<BYTE*>(dwAddress + i), pbMask, szMask))
                    return dwAddress + i;
            }
            return 0;
        }
        template<typename T> T RPM(uintptr_t address) {
            return *(T*)address;
        }
        template<typename T> T WPM(uintptr_t address, int val) {
            T buffer = (T)val;
            *(T*)address = buffer;
            return *(T*)address;
        }
    };

}

extern Memory::External ext;

extern Memory::Internal in;