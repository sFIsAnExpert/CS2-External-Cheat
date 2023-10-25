#include "Aimbot.h"

AimManager _aim;

CEntity AimManager::BestEntity(bool isVis)
{
    int max_dist = 999999, i = 0, entAt = 0;
    CEntity badEnt; badEnt.Controller.AimData = CEntityState::BAD;

    std::vector<CEntity> list = {};
    ViewMatrix vm = engine.Player.View.GetViewMatrix(); Resolution res = engine.Screen.GetResolution();
    for (const CEntity& entity : engine.Server.World.EntityList()) {
        if (entity.Pawn.BoneData.BonePosList.empty())
            continue;

        if (isVis) {
            if (!engine.Utils.isVisible(entity))
                continue;
        }
            
        Vector2 screenpos = entity.Pawn.BoneData.BonePosList[BONEINDEX::head].ScreenPos;
        float CenterX = res.width / 2, CenterY = res.height / 2;

        float dist = math.Dist2D(screenpos, { CenterX, CenterY });
        if (dist <= cyrix.fRadius)
            list.push_back(entity);
    }

    if (!list.empty()) {
        for (const CEntity& entity : list) {
            float dist = math.Dist3D(engine.Player.LocalPlayer.Pawn.Position, entity.Pawn.Position);
            if (dist < max_dist) {
                max_dist = dist;
                entAt = i;
            }
            i++;
        }
        return list.at(entAt);
    }

    return badEnt;
}

CEntity AimManager::RageBestEntity()
{
    int max_dist = 999999, crosshair_max = 9999999, i = 0, entAt = 0; Resolution res = engine.Screen.GetResolution();
    CEntity badEnt; badEnt.Controller.AimData = CEntityState::BAD; bool isVis = false;
    std::vector<CEntity> list = engine.Server.World.EntityList();
    for (const CEntity& entity : list) {
        if (entity.Pawn.BoneData.BonePosList.empty())
            continue;
        int dist = (int)(math.Dist3D(engine.Player.LocalPlayer.Pawn.Position, entity.Pawn.Position));
        if (engine.Utils.isVisible(entity)) {
            isVis = true;
            Vector2 screenpos = entity.Pawn.BoneData.BonePosList[BONEINDEX::head].ScreenPos;
            float CenterX = res.width / 2, CenterY = res.height / 2;
            float crossDist = math.Dist2D(screenpos, { CenterX, CenterY });
            if (crossDist <= crosshair_max) {
                crosshair_max = crossDist;
                entAt = i;
            }
        }
        else if (dist < max_dist && !isVis) {
            max_dist = dist;
            entAt = i;
        }

        i++;
    }

    if (max_dist < 999999 || crosshair_max < 999999)
        return list.at(entAt);

    return badEnt;
}

void AimManager::AimAtPos(float x, float y, Resolution res)
{
    int ScreenCenterX = res.width / 2, ScreenCenterY = res.height / 2;
    float TargetX = 0;
    float TargetY = 0;
    float AimSpeed = cyrix.fAimSpeed;

    if (x != 0)
    {
        if (x > ScreenCenterX)
        {
            TargetX = -(ScreenCenterX - x);
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
        }

        if (x < ScreenCenterX)
        {
            TargetX = x - ScreenCenterX;
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX < 0) TargetX = 0;
        }
    }

    if (y != 0)
    {
        if (y > ScreenCenterY)
        {
            TargetY = -(ScreenCenterY - y);
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
        }

        if (y < ScreenCenterY)
        {
            TargetY = y - ScreenCenterY;
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY < 0) TargetY = 0;
        }
    }

    TargetX /= 10;
    TargetY /= 10;

    if (fabs(TargetX) < 1)
    {
        if (TargetX > 0)
        {
            TargetX = 1;
        }
        if (TargetX < 0)
        {
            TargetX = -1;
        }
    }
    if (fabs(TargetY) < 1)
    {
        if (TargetY > 0)
        {
            TargetY = 1;
        }
        if (TargetY < 0)
        {
            TargetY = -1;
        }
    }

    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dx = static_cast<DWORD>(TargetX);
    input.mi.dy = static_cast<DWORD>(TargetY);
    input.mi.mouseData = 0;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.time = 0;
    input.mi.dwExtraInfo = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void AimManager::StandardAim()
{
    if (GetAsyncKeyState(VK_LCONTROL) || GetAsyncKeyState(VK_LSHIFT)) {
        CEntity entity = BestEntity(cyrix.bVisible);
        if (entity.Controller.AimData != CEntityState::BAD) {
            Vector2 wHead = entity.Pawn.BoneData.BonePosList[head].ScreenPos;
            Resolution res = engine.Screen.GetResolution();
            AimAtPos(wHead.x, wHead.y, res);
            Sleep(cyrix.fAimDelay);
        }
    }
}

void AimManager::Aimbot(const CEntity& Local, Vector3 LocalPos, Vector3 AimPos)
{
    float Yaw, Pitch;
    float Distance, Norm;
    Vector3 OppPos;

    OppPos = AimPos - LocalPos;

    Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

    Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513 - Local.Pawn.ViewAngles.y;
    Pitch = -atan(OppPos.z / Distance) * 57.295779513 - Local.Pawn.ViewAngles.x;
    Norm = sqrt(pow(Yaw, 2) + pow(Pitch, 2));

    Yaw = Yaw * cyrix.fSmooth + Local.Pawn.ViewAngles.y;
    Pitch = Pitch * cyrix.fSmooth + Local.Pawn.ViewAngles.x;
    Vector2 Angle{ Pitch,Yaw };
    ext.WPM<Vector2>(engine.ClientBase + offsets.Client.dwViewAngles, Angle);
}

void AimManager::RageAim()
{
    if (GetAsyncKeyState(VK_LCONTROL) || GetAsyncKeyState(VK_LSHIFT)) {
        CEntity entity = RageBestEntity();
        if (entity.Controller.AimData != CEntityState::BAD) {
            Vector3 wHead = entity.Pawn.BoneData.BonePosList[BONEINDEX::head].Pos; 
            //Aimbot(engine.Player.LocalPlayer, engine.Player.LocalPlayer.Pawn.ViewOrigin, wHead);
            uintptr_t vAngleAddr = engine.ClientBase + offsets.Client.dwViewAngles;
            Vector3 AimAngles, vAngles = ext.RPM<Vector3>(vAngleAddr);
            math.CalculateAimAngles(&wHead, &engine.Player.LocalPlayer.Pawn.ViewOrigin, &AimAngles);
            AimAngles.x -= engine.Player.LocalPlayer.Pawn.PunchAngle.x; 
            AimAngles.y -= engine.Player.LocalPlayer.Pawn.PunchAngle.y;
            math.ClampAngles(AimAngles);

            ext.WPM<float>(vAngleAddr, AimAngles.x);
            ext.WPM<float>(vAngleAddr + 0x4, AimAngles.y);
        }
    }
}
