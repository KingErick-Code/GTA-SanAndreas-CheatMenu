#pragma once

#include <memory>
#include <d3d11.h>
#include <windows.h>
#include "../ui/mod_menu.h"
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_dx11.h"
#include "../../include/imgui/imgui_impl_win32.h"

class App
{
public:
	App();
	~App();

	int init();
private:
	void doFrame();
private:
	HWND window = nullptr;

	int width = 800;
	int	height = 600;

	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTarget = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;

	std::unique_ptr<Menu> menu;
};

