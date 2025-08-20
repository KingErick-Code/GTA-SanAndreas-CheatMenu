#pragma once

#include "../../include/imgui/imgui.h"
#include <windows.h>
#include <vector>

#define OPTIONS_LIST \
		X(Player) \
		X(Vehicle)

enum OPTION {
#define X(name) name,
	OPTIONS_LIST
#undef X
	OPTION_COUNT
};

constexpr const char* OPTION_NAMES[] = {
#define X(name) #name,
	OPTIONS_LIST
#undef X
};

class Menu
{
public:
	Menu(HWND window);
	void render();
private:
	HWND window;

	OPTION currentOption = Player;
private:
	void sideBar(ImVec2 sideBarSize, ImVec2 buttonSize);
	void mainPanel();
	void centerText(const char* text);

	void playerMods();
};