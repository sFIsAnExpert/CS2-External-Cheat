#include "Drawing.h"

ImVec2 Drawing::vWindowSize = { 750, 500 };
ImGuiWindowFlags Drawing::WindowFlags = 0;
bool Drawing::bDraw = true;

bool Drawing::isActive()
{
	return bDraw == true;
}

void Drawing::Draw()
{
	if (isActive())
	{
		ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(Xor("Cyrix - ChasePlays"), nullptr, WindowFlags);
		{
			if(ImGui::Button(Xor("Visuals"), { 75,25 })){
				cyrix.Tab = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button(Xor("Aim"), { 75,25 })) {
				cyrix.Tab = 2;
			}
			if (cyrix.Tab == 1) {
				ImGui::Checkbox(Xor("3D Box"), &cyrix.b3DEsp);
				ImGui::Checkbox(Xor("ESP"), &cyrix.bEsp);
				if (cyrix.bEsp) {
					ImGui::SameLine();
					ImGui::Checkbox(Xor("Filled"), &cyrix.bFilled);
				}
				ImGui::Checkbox(Xor("Draw Bones"), &cyrix.bDrawBone);
				ImGui::Checkbox(Xor("Draw Names"), &cyrix.bNameEsp);
				ImGui::Checkbox(Xor("Team Check"), &cyrix.bTeamCheck);
			}
			if (cyrix.Tab == 2) {
				ImGui::Checkbox(Xor("Rage Aimbot"), &cyrix.bRageAim);
				ImGui::Checkbox(Xor("Aimbot"), &cyrix.bAimbot);
				if (cyrix.bAimbot) {
					ImGui::SameLine();
					ImGui::Checkbox("Visible", &cyrix.bVisible);
				}
				if (cyrix.bAimbot || cyrix.bRageAim) {
					ImGui::SliderFloat(Xor("Smoothing"), &cyrix.fSmooth, 0, 100);
				}
				ImGui::SliderFloat(Xor("Radius"), &cyrix.fRadius, 0, 150);
				ImGui::SliderFloat(Xor("Aim Speed"), &cyrix.fAimSpeed, 0, 100);
				ImGui::SliderFloat(Xor("Aim Delay"), &cyrix.fAimDelay, 0, 1);
				ImGui::NewLine();
			}

			_renderer.Render();
		}
		ImGui::End();
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
}