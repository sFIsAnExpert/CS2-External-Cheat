#pragma once
#include "Engine.h"

CS2 engine;

bool UpdateAllBoneData(const uintptr_t* EntityPawnAddress, CBone* Bone)
{
	if (*EntityPawnAddress == 0)
		return false;
	 Bone->EntityPawnAddress = *EntityPawnAddress;


	uintptr_t GameSceneNode = ext.RPM<uintptr_t>(*EntityPawnAddress + offsets.Pawn.GameSceneNode);
	uintptr_t BoneArrayAddress = ext.RPM<uintptr_t>(GameSceneNode + offsets.Pawn.BoneArray);

	BoneJointData BoneArray[30]{};
	ReadProcessMemory(ext.ProcessInfo.hHand, (BYTE*)BoneArrayAddress, BoneArray, 30 * sizeof(BoneJointData), nullptr);

	for (int i = 0; i < 30; i++)
	{
		Vector2 ScreenPos; ViewMatrix vm = engine.Player.View.GetViewMatrix();
		bool IsVisible = false; Resolution res = engine.Screen.GetResolution();

		if (WorldToScreen(BoneArray[i].Pos, ScreenPos, vm, res))
			IsVisible = true;

		Bone->BonePosList.push_back({ BoneArray[i].Pos ,ScreenPos,IsVisible });
	}

	return Bone->BonePosList.size() > 0;
}

std::vector<CEntity> CS2::Server::World::EntityList()
{
	std::vector<CEntity> list;

	uintptr_t LocalController = ext.RPM<uintptr_t>(engine.ClientBase + offsets.Client.dwLocalPlayerController);
	uintptr_t LocalPawn = ext.RPM<uintptr_t>(engine.ClientBase + offsets.Client.dwLocalPlayerPawn);

	if (LocalPawn != 0 && LocalController != 0)
		engine.CreateLocalPlayer(&LocalController, &LocalPawn);

	for (int i = 0; i < 64; i++) {
		uintptr_t EntityController = ext.RPM<uintptr_t>((uintptr_t)engine.EntitySystem.dwEntityList + (i + 1) * 0x78);

		if (EntityController == 0)
			continue;

		if (EntityController == LocalController) {
			engine.Player.LocalPlayer.Index = i;
			continue;
		}
		DWORD PawnHandle = ext.RPM<DWORD>(EntityController + offsets.Controller.PlayerPawn);
		uintptr_t EntityPawn = engine.Server.PlayerController.GetPawnAddress(PawnHandle);
	
		CEntity entity = engine.CreateEntity(&EntityController, &EntityPawn);
		entity.Index = i;

		if (cyrix.bTeamCheck && entity.Controller.TeamID == engine.Player.LocalPlayer.Controller.TeamID)
			continue;

		if(entity.Controller.Alive)
			list.push_back(entity);

	}

	return list;
}

void CS2::CreateLocalPlayer(uintptr_t* base, uintptr_t* pawn) {

	engine.Player.LocalPlayer.Controller.TeamID = ext.RPM<int>(*base + offsets.Controller.TeamID);
	engine.Player.LocalPlayer.Controller.Pawn = ext.RPM<DWORD>(*base + offsets.Controller.PlayerPawn);
	engine.Player.LocalPlayer.Controller.Address = *base;

	engine.Player.LocalPlayer.Pawn.PunchAngle = ext.RPM<Vector3>(*pawn + offsets.Pawn.aimPunchAngle);
	engine.Player.LocalPlayer.Pawn.ViewOrigin = ext.RPM<Vector3>(*pawn + offsets.Pawn.vecLastClipCameraPos);
	engine.Player.LocalPlayer.Pawn.ViewAngles = ext.RPM<Vector3>(*pawn + offsets.Pawn.angEyeAngles);
	engine.Player.LocalPlayer.Pawn.SpottedMask = ext.RPM<uintptr_t>(*pawn + offsets.Pawn.bSpottedByMask);
	engine.Player.LocalPlayer.Pawn.Position = ext.RPM<Vector3>(*pawn + offsets.Pawn.Pos);
	engine.Player.LocalPlayer.Pawn.Address = *pawn;
}

CEntity CS2::CreateEntity(uintptr_t* base, uintptr_t* pawn)
{
	CEntity entity;
		
	entity.Controller.Health = ext.RPM<int>(*base + offsets.Controller.iPawnHealth);
	entity.Controller.Armor = ext.RPM<int>(*base + offsets.Controller.iPawnArmor);
	entity.Controller.Alive = ext.RPM<bool>(*base + offsets.Controller.IsAlive);
	entity.Controller.TeamID = ext.RPM<int>(*base + offsets.Controller.TeamID);
	entity.Controller.Pawn = ext.RPM<DWORD>(*base + offsets.Controller.PlayerPawn);
	entity.Controller.Address = *base;
	ReadProcessMemory(ext.ProcessInfo.hHand, (BYTE*)*base + offsets.Controller.iszPlayerName, entity.Controller.Name, 32, nullptr);

	entity.Pawn.Immune = ext.RPM<bool>(*pawn + offsets.Pawn.bGunGameImmunity);
	entity.Pawn.PunchAngle = ext.RPM<Vector3>(*pawn + offsets.Pawn.aimPunchAngle);
	entity.Pawn.TeamID = ext.RPM<int>(*pawn + offsets.Pawn.iTeamNum);
	entity.Pawn.ViewOrigin = ext.RPM<Vector3>(*pawn + offsets.Pawn.vecLastClipCameraPos);
	entity.Pawn.ViewAngles = ext.RPM<Vector3>(*pawn + offsets.Pawn.angEyeAngles);
	entity.Pawn.SpottedMask = ext.RPM<uintptr_t>(*pawn + offsets.Pawn.bSpottedByMask);
	entity.Pawn.Position = ext.RPM<Vector3>(*pawn + offsets.Pawn.Pos);
	entity.Pawn.Address = *pawn;

	UpdateAllBoneData(pawn, &entity.Pawn.BoneData);

	return entity;
}

void CS2::InitEntry()
{
	this->ClientBase = ext.GetModuleBase(L"client.dll");
	this->Engine2Base = ext.GetModuleBase(L"engine2.dll");
	this->Entry = ext.RPM<GameEntry>(this->ClientBase + offsets.Client.dwEntityList);
	this->EntitySystem = ext.RPM<CGameEntitySystem>((uintptr_t)this->Entry.dwEntitySystem);
}

uintptr_t CS2::Server::CSPlayerController::GetPawnAddress(DWORD Pawn)
{
	uintptr_t PawnListEntry = ext.RPM<uintptr_t>(engine.ClientBase + offsets.Client.dwEntityList);
	PawnListEntry = ext.RPM<uintptr_t>(PawnListEntry + 0x10 + 8 * ((Pawn & 0x7FFF) >> 9));
	return ext.RPM<uintptr_t>(PawnListEntry + 0x78 * (Pawn & 0x1FF));
}

ViewMatrix CS2::Player::View::GetViewMatrix()
{
	return ext.RPM<ViewMatrix>(engine.ClientBase + offsets.Client.dwViewMatrix);
}

Resolution CS2::Screen::GetResolution()
{
	return ext.RPM<Resolution>(engine.Engine2Base + offsets.Engine2.dwWindowResolution);
}

bool CS2::Utils::isVisible(CEntity entity)
{
	if (entity.Pawn.SpottedMask & (DWORD64(1) << (engine.Player.LocalPlayer.Index)) || engine.Player.LocalPlayer.Pawn.SpottedMask & (DWORD64(1) << (entity.Index))) {
		return true;
	}
	return false;
}
