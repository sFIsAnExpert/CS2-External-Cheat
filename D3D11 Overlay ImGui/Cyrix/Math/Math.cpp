#pragma once
#include "Math.h"

XMath math;

bool WorldToScreen(Vector3 Pos, Vector2& screen, ViewMatrix vm, Resolution res) {
    float View = 0.f;
    float SightX = res.width / 2, SightY = res.height / 2;

    View = vm.matrix[3][0] * Pos.x + vm.matrix[3][1] * Pos.y + vm.matrix[3][2] * Pos.z + vm.matrix[3][3];

    if (View <= 0.01)
        return false;

    screen.x = SightX + (vm.matrix[0][0] * Pos.x + vm.matrix[0][1] * Pos.y + vm.matrix[0][2] * Pos.z + vm.matrix[0][3]) / View * SightX;
    screen.y = SightY - (vm.matrix[1][0] * Pos.x + vm.matrix[1][1] * Pos.y + vm.matrix[1][2] * Pos.z + vm.matrix[1][3]) / View * SightY;

    return true;
}

bool WorldToScreen(Vector3 Pos, ImVec2& screen, ViewMatrix vm, Resolution res) {
    float View = 0.f;
    float SightX = res.width / 2, SightY = res.height / 2;

    View = vm.matrix[3][0] * Pos.x + vm.matrix[3][1] * Pos.y + vm.matrix[3][2] * Pos.z + vm.matrix[3][3];

    if (View <= 0.01)
        return false;

    screen.x = SightX + (vm.matrix[0][0] * Pos.x + vm.matrix[0][1] * Pos.y + vm.matrix[0][2] * Pos.z + vm.matrix[0][3]) / View * SightX;
    screen.y = SightY - (vm.matrix[1][0] * Pos.x + vm.matrix[1][1] * Pos.y + vm.matrix[1][2] * Pos.z + vm.matrix[1][3]) / View * SightY;

    return true;
}

void Draw3DBox(Vector3 HeadPos, Vector3 Pos, ViewMatrix vm, Resolution res, ImColor color) {
    Vector3 tfl = Vector3{ HeadPos.x + 15, HeadPos.y - 15, HeadPos.z };
    Vector3 tfr = Vector3{ HeadPos.x + 15, HeadPos.y + 15, HeadPos.z };
    Vector3 tbl = Vector3{ HeadPos.x - 15, HeadPos.y + 15, HeadPos.z };
    Vector3 tbr = Vector3{ HeadPos.x - 15, HeadPos.y - 15, HeadPos.z };

    ImVec2 tfl2D, tfr2D, tbl2D, tbr2D;
    if (!WorldToScreen(tfl, tfl2D, vm, res)) { return; }
    if (!WorldToScreen(tfr, tfr2D, vm, res)) { return; }
    if (!WorldToScreen(tbl, tbl2D, vm, res)) { return; }
    if (!WorldToScreen(tbr, tbr2D, vm, res)) { return; }

    Vector3 bfl = Vector3{ Pos.x + 15, Pos.y - 15, Pos.z };
    Vector3 bfr = Vector3{ Pos.x + 15, Pos.y + 15, Pos.z };
    Vector3 bbl = Vector3{ Pos.x - 15, Pos.y + 15, Pos.z };
    Vector3 bbr = Vector3{ Pos.x - 15, Pos.y - 15, Pos.z };

    ImVec2 bfl2D, bfr2D, bbl2D, bbr2D;
    if (!WorldToScreen(bfl, bfl2D, vm, res)) { return; }
    if (!WorldToScreen(bfr, bfr2D, vm, res)) { return; }
    if (!WorldToScreen(bbl, bbl2D, vm, res)) { return; }
    if (!WorldToScreen(bbr, bbr2D, vm, res)) { return; }

    ImGui::GetBackgroundDrawList()->AddLine(tbl2D, tfr2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(tfl2D, tfr2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(tfl2D, tbr2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(tbl2D, tbr2D, color, 1.f);

    ImGui::GetBackgroundDrawList()->AddLine(bbl2D, bfr2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(bfr2D, bfl2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(bfl2D, bbr2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(bbr2D, bbl2D, color, 1.f);

    ImGui::GetBackgroundDrawList()->AddLine(bbr2D, tbr2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(bbl2D, tbl2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(bfr2D, tfr2D, color, 1.f);
    ImGui::GetBackgroundDrawList()->AddLine(bfl2D, tfl2D, color, 1.f);
}

void DrawNormalBox(float x, float y, float w, float h, float thickness, ImVec4 borderColor)
{
    ImVec2 topLeft(x, y);
    ImVec2 bottomRight(x + w, y + h);

    ImGui::GetBackgroundDrawList()->AddRect(topLeft, bottomRight, ImColor(borderColor), 0.0f, 15, thickness);
}

void DrawFilledBox(float x, float y, float w, float h, ImVec4 color) {
    ImVec2 topLeft(x, y);
    ImVec2 bottomRight(x + w, y + h);
    ImGui::GetBackgroundDrawList()->AddRectFilled(topLeft, bottomRight, ImColor(color));
}

float XMath::Dist2D(Vector2 from, Vector2 to) {
    float val = ((from.x - to.x) * (from.x - to.x) + ((from.y - to.y) * (from.y - to.y)));

    return sqrt(val);
}

float XMath::Dist3D(Vector3 from, Vector3 to) {
    float val = ((to.x - from.x) * (to.x - from.x)) + ((to.y - from.y) * (to.y - from.y)) + ((to.z - from.z) * (to.z - from.z));

    return sqrt(val);
}

float XMath::DotProduct3D(Vector3 left, Vector3 right)
{
    return (left.x * right.x + left.y * right.y + left.z * right.z);
}

float XMath::DotProduct2D(Vector2 left, Vector2 right)
{
    return (left.x * right.x + left.y * right.y);
}

float XMath::VectorLength3D(Vector3 left, Vector3 right)
{
    return sqrtf(DotProduct3D(left, right));
}

float XMath::VectorLength2D(Vector2 left, Vector2 right)
{
    return sqrtf(DotProduct2D(left, right));
}

void XMath::VectorNormalize(Vector3& direction)
{
    float flLen = VectorLength3D(direction, direction);

    if (flLen == 0.0f)
    {
        direction.x = 0.0f;
        direction.y = 0.0f;
        direction.z = 1.0f;
    }

    else
    {
        flLen = 1.0f / flLen;

        direction *= flLen;
    }
}

void XMath::ClampAngles(Vector3& angles)
{
    while (angles.y > 180.0f)
        angles.y -= 360.0f;

    while (angles.y < -180.0f)
        angles.y += 360.0f;

    while (angles.x > 89.0f)
        angles.x -= 360.0f;

    while (angles.x < -89.0f)
        angles.x += 360.0f;

    if (angles.z != 0.0f)
        angles.z = 0.0f;
}

void XMath::VectorAngles(Vector3 direction, Vector3& angles)
{
    float flTemp, flYaw, flPitch;

    if (direction.x == 0.0f && direction.y == 0.0f)
    {
        flYaw = 0.0f;

        if (direction.z > 0.0f)
            flPitch = 90.0f;

        else
            flPitch = 270.0f;
    }

    else
    {
        flYaw = RadiansToDegrees(atan2f(direction.y, direction.x));

        if (flYaw < 0.0f)
            flYaw += 360.0f;

        flTemp = sqrtf(direction.x * direction.x + direction.y * direction.y);
        flPitch = RadiansToDegrees(atan2f(direction.z, flTemp));

        if (flPitch < 0.0f)
            flPitch += 360.0f;
    }

    angles.x = -flPitch;
    angles.y = flYaw;
    angles.z = 0.0f;
}

void XMath::CalculateAimAngles(Vector3* vTargetPos, Vector3* vCameraPos, Vector3* vAngles)
{
    Vector3 vDelta;
    vDelta.x = vTargetPos->x - vCameraPos->x;
    vDelta.y = vTargetPos->y - vCameraPos->y;
    vDelta.z = vTargetPos->z - vCameraPos->z;
    VectorNormalize(vDelta);
    VectorAngles(vDelta, *vAngles);
}