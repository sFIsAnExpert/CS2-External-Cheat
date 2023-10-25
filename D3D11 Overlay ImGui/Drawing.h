#ifndef DRAWING_H
#define DRAWING_H

#pragma once
#include "pch.h"
#include "UI.h"

class Drawing
{
private:
	static ImVec2 vWindowSize;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;

public:
	static bool isActive();
	static void Draw();
};

#endif