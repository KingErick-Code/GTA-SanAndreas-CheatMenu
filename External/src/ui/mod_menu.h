#pragma once

#include "../../include/imgui/imgui.h"
#include <windows.h>

class Menu
{
public:
	Menu(HWND window);
	void render();
private:
	HWND window;
};