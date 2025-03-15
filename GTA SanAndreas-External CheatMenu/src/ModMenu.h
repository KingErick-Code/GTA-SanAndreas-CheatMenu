#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <vector>

void CenterText(const char* text);
void LoadCustomFont(ImGuiIO& io);
void SetCustomTheme();
void SideBar(const std::vector<const char*>& labels, const ImVec2& sideBarSize, ImVec2 buttonSize);
void RenderModMenu();