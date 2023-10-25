#ifndef PCH_H
#define PCH_H

#include <Windows.h>
#include <dwmapi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <list>
#include <tchar.h>
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <TlHelp32.h>
#include "Xor.h"
#include "d3d11.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_win32.h"

struct Cyrix {
	bool bEsp = false, bNameEsp = false, bFilled = false, b3DEsp = false, bTeamCheck = false, bDrawBone = false, bAimbot = false, bVisible = false;
	bool bTriggerbot = false, bRecoil = false, bRageAim = false;
	float fRadius = 45.f, fAimSpeed = 50.f, fAimDelay = 0.8f, fSmooth = 0.7f;
	__int8 Tab = 1;
};

extern Cyrix cyrix;

#endif
