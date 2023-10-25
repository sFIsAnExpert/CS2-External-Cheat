#pragma once
#include "../../pch.h"
#include "../Engine/Engine.h"

struct AimManager {
private:
    CEntity BestEntity(bool);
    CEntity RageBestEntity();
    void AimAtPos(float x, float y, Resolution res);
    void Aimbot(const CEntity& Local, Vector3 LocalPos, Vector3 AimPos);
public:
    void StandardAim();
    void RageAim();
};

extern AimManager _aim;