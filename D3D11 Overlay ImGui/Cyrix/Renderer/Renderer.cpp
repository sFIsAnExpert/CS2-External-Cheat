#pragma once
#include "Renderer.h"

Renderer _renderer;

void Renderer::DrawBone(const CEntity& Entity, ImColor Color, float Thickness)
{
	if (Entity.Pawn.BoneData.BonePosList.empty())
		return;
	BoneJointPos Previous, Current;
	for (auto i : BoneJointList::List)
	{
		Previous.Pos = Vector3(0, 0, 0);
		for (auto Index : i)
		{
			Current = Entity.Pawn.BoneData.BonePosList[Index];
			if (Previous.Pos.x == 0.f && Previous.Pos.y == 0.f && Previous.Pos.z == 0.f)
			{
				Previous = Current;
				continue;
			}
			if (Previous.IsVisible && Current.IsVisible)
			{
				ImGui::GetBackgroundDrawList()->AddLine({ Previous.ScreenPos.x, Previous.ScreenPos.y }, { Current.ScreenPos.x, Current.ScreenPos.y }, Color, Thickness);
			}
			Previous = Current;
		}
	}
}

void Renderer::Render() {
	for (const CEntity& entity : engine.Server.World.EntityList()) {
		Vector3 pos = entity.Pawn.Position, headPos = pos; headPos.z += 75.f; 
		Vector2 wPos, wHead; ViewMatrix vm = engine.Player.View.GetViewMatrix(); Resolution res = engine.Screen.GetResolution();
		if (WorldToScreen(pos, wPos, vm, res) && WorldToScreen(headPos, wHead, vm, res)) {
			float height = wHead.y - wPos.y, y = wPos.y;
			float width = height / 2.0f, x = wPos.x - (width / 2.0f);

			if (cyrix.b3DEsp)
				Draw3DBox(pos, headPos, vm, res, ImColor(255, 0, 0));
			if(cyrix.bEsp)
				DrawNormalBox(x, y, width, height, 1, ImColor(255, 0, 0));
			if (cyrix.bFilled)
				DrawFilledBox(x, y, width, height, ImVec4(0, 0, 0, .55f));
			if (cyrix.bNameEsp)
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(wHead.x - 18.f, wHead.y - 13.f), ImColor(0, 0, 0), entity.Controller.Name);
			if (cyrix.bDrawBone)
				DrawBone(entity, ImColor(255, 255, 255), 1.3f);
		}
	}
}
