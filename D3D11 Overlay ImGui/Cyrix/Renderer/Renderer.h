#pragma once
#include "../Engine/Engine.h"

struct Renderer {
	void Render();
	void DrawBone(const CEntity& Entity, ImColor Color, float Thickness);
};

extern Renderer _renderer;
