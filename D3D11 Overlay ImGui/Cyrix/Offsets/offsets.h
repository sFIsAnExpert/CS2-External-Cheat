#pragma once
#include "../../pch.h"
#include "../Memory/Memory.h"
#include "../Math/Math.h"

class Offsets {
public:
	struct Client {
		uintptr_t dwBaseEntityModel_setModel = 0x57DA70;
		uintptr_t dwEntityList = 0x1798738;
		uintptr_t dwForceBackward = 0x169E120;
		uintptr_t dwForceCrouch = 0x169E3F0;
		uintptr_t dwForceForward = 0x169E090;
		uintptr_t dwForceJump = 0x169E360;
		uintptr_t dwForceLeft = 0x169E1B0;
		uintptr_t dwForceRight = 0x169E240;
		uintptr_t dwGameEntitySystem_getBaseEntity = 0x601070;
		uintptr_t dwGameEntitySystem_getHighestEntityIndex = 0x5F2D60;
		uintptr_t dwGameRules = 0x17F4478;
		uintptr_t dwGlobalVars = 0x1699F40;
		uintptr_t dwGlowManager = 0x17F3BF8;
		uintptr_t dwInterfaceLinkList = 0x197F2B8;
		uintptr_t dwLocalPlayerController = 0x17E7158;
		uintptr_t dwLocalPlayerPawn = 0x1885C28;
		uintptr_t dwPlantedC4 = 0x188CAD0;
		uintptr_t dwViewAngles = 0x18E5740;
		uintptr_t dwViewMatrix = 0x1886710;
		uintptr_t dwViewRender = 0x1887108;
	};
	struct Controller {
		DWORD TeamID = 0x3BF;
		DWORD IsAlive = 0x7C4;
		DWORD PlayerPawn = 0x7BC;
		DWORD iszPlayerName = 0x610;
		DWORD iPawnArmor = 0x7CC;
		DWORD iPawnHealth = 0x7C8;
		DWORD bPawnHasDefuser = 0x7D0;
		DWORD iszClan = 0x718;
	};
	struct Pawn {
		DWORD Pos = 0x1214;
		DWORD MaxHealth = 0x328;
		DWORD CurrentHealth = 0x32C;
		DWORD GameSceneNode = 0x310;
		DWORD BoneArray = 0x1E0;
		DWORD angEyeAngles = 0x14F8;
		DWORD vecLastClipCameraPos = 0x1274;
		DWORD iShotsFired = 0x1400;
		DWORD flFlashDuration = 0x1450;
		DWORD aimPunchAngle = 0x16FC;
		DWORD aimPunchCache = 0x1720;
		DWORD iTeamNum = 0x3BF;
		DWORD CameraServices = 0x10E0;
		DWORD iFovStart = 0x214;
		DWORD fFlags = 0x3C8;
		DWORD pGlowServices = 0x16B0;
		DWORD iArmorValue = 0x14F0;
		DWORD iIDEntIndex = 0x1524;
		DWORD bIsScoped = 0x1388;
		DWORD bIsWalking = 0x1389;
		DWORD bIsDefusing = 0x1390;
		DWORD bGunGameImmunity = 0x13A4;
		DWORD bSpottedByMask = 0x1618 + 0xC;
		DWORD pClippingWeapon = 0x1290;
	};
	struct Engine2 {
		uintptr_t dwBuildNumber = 0x487514;
		uintptr_t dwNetworkGameClient = 0x486AB0;
		uintptr_t dwNetworkGameClient_getLocalPlayer = 0xF0;
		uintptr_t dwNetworkGameClient_maxClients = 0x250;
		uintptr_t dwNetworkGameClient_signOnState = 0x240;
		uintptr_t dwWindowResolution = 0x538678;
	};
	struct CSWeaponBase {
		DWORD bPlayerFireEventIsPrimary = 0x15DC;
		DWORD fAccuracyPenalty = 0x1670;
		DWORD flTurningInaccuracyDelta = 0x165C;
		DWORD flRecoilIndex = 0x1684;
		DWORD flGunAccuracyPosition = 0x161C;
	};
	CSWeaponBase CSWeapon;
	Engine2 Engine2;
	Client Client;
	Pawn Pawn;
	Controller Controller;
};

enum BONEINDEX : DWORD
{
	head = 6,
	neck_0 = 5,
	spine_1 = 4,
	spine_2 = 2,
	pelvis = 0,
	arm_upper_L = 8,
	arm_lower_L = 9,
	hand_L = 10,
	arm_upper_R = 13,
	arm_lower_R = 14,
	hand_R = 15,
	leg_upper_L = 22,
	leg_lower_L = 23,
	ankle_L = 24,
	leg_upper_R = 25,
	leg_lower_R = 26,
	ankle_R = 27,
};

enum CEntityState {
	DEAD,
	ALIVE,
	BAD = 9501231
};

struct BoneJointData
{
	Vector3 Pos;
	char pad[0x14];
};

struct BoneJointPos
{
	Vector3 Pos;
	Vector2 ScreenPos;
	bool IsVisible = false;
};

namespace BoneJointList
{
	inline std::list<DWORD> Trunk = { head,neck_0,spine_2, pelvis };
	inline std::list<DWORD> LeftArm = { neck_0,  arm_upper_L, arm_lower_L, hand_L };
	inline std::list<DWORD> RightArm = { neck_0, arm_upper_R,arm_lower_R, hand_R };
	inline std::list<DWORD> LeftLeg = { pelvis, leg_upper_L , leg_lower_L, ankle_L };
	inline std::list<DWORD> RightLeg = { pelvis, leg_upper_R , leg_lower_R, ankle_R };
	inline std::vector<std::list<DWORD>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };
}

struct CBone
{
public:
	uintptr_t EntityPawnAddress = 0;
	std::vector<BoneJointPos> BonePosList;
};

struct CSPlayerPawn {
	Vector3 Position, ViewOrigin, PunchAngle, ViewAngles;
	int TeamID;
	bool Immune;
	CBone BoneData;
	uintptr_t SpottedMask, Address;
};

struct CSPlayerController {
	uintptr_t Address = 0;
	char Name[32];
	int Health = 0, Armor = 0, TeamID = 0, Pawn = 0, Alive = 0, AimData = 0;
};

struct CEntity {
	CSPlayerController Controller;
	CSPlayerPawn Pawn;
	int Index;
};

struct CGameEntitySystem {
	char pad_0x10[0x10];
	uintptr_t* dwEntityList;
};

struct GameEntry {
	CGameEntitySystem* dwEntitySystem;
};

extern Offsets offsets;