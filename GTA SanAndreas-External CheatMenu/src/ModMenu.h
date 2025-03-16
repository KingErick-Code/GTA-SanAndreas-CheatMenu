#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "./Memory.h"
#include <vector>

class Memory;

void CenterText(const char* text);
void LoadCustomFont(ImGuiIO& io);
void SetCustomTheme();
void SideBar(const std::vector<const char*>& labels, const ImVec2& sideBarSize, ImVec2 buttonSize);
void RenderModMenu();

enum Active
{
	Player,
	Vehicle
};

struct CVector {
	float x;
	float y;
	float z;

	CVector(float x, float y, float z) : x(x), y(y), z(z) {

	}
};

extern Active ModOptions;
extern Memory memory;