#include "gui.h"
#include <Windows.h>
#include <tchar.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "ModMenu.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LPDIRECT3D9 GUI::d3d = nullptr;
LPDIRECT3DDEVICE9 GUI::d3dDevice = nullptr;
bool GUI::deviceLost = false;
UINT GUI::resizeWidth = 800;
UINT GUI::resizeHeight = 600;
D3DPRESENT_PARAMETERS GUI::d3dpp = {};
Memory GUI::memory;
bool messageShown;

bool GUI::Initialize(HWND hWnd) {
	if (!CreateDeviceD3D(hWnd)) return false;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(d3dDevice);

	io.Fonts->AddFontDefault();
	LoadCustomFont(io);
	SetCustomTheme();

	messageShown = false;
	return true;
}

void GUI::Shutdown() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
}

void GUI::Render() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderModMenu();

	ImGui::Render();
	d3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(114, 114, 154, 255);
	d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

	if (d3dDevice->BeginScene() >= 0) {
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		d3dDevice->EndScene();
	}
	if (d3dDevice->Present(nullptr, nullptr, nullptr, nullptr) == D3DERR_DEVICELOST) deviceLost = true;
}

void GUI::ResetDevice() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = d3dDevice->Reset(&d3dpp);
	if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

void GUI::ProcessInput() {
	if (deviceLost) {
		HRESULT hr = d3dDevice->TestCooperativeLevel();
		if (hr == D3DERR_DEVICELOST) {
			::Sleep(10);
			return;
		}
		if (hr == D3DERR_DEVICENOTRESET) ResetDevice();
		deviceLost = false;
	}
}

bool GUI::CreateDeviceD3D(HWND hWnd) {
	if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr) return false;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice) < 0) return false;
	return true;
}
void GUI::CleanupDeviceD3D() {
	if (d3dDevice) {
		d3dDevice->Release();
		d3dDevice = nullptr;
	}

	if (d3d) {
		d3d->Release();
		d3d = nullptr;
	}
}
LRESULT WINAPI GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

	switch (msg){
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) return 0;
		resizeWidth = (UINT)LOWORD(lParam);
		resizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_GETMINMAXINFO: {
		MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		minMaxInfo->ptMinTrackSize.x = 800; // Min width
		minMaxInfo->ptMinTrackSize.y = 600; // Min height
	}break;
	}

	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
