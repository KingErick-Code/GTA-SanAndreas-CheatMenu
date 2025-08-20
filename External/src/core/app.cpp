#include "app.h"
#include <windowsx.h>

LRESULT WINAPI windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


App::App()
{
	WNDCLASSEX windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = windowProc;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.lpszClassName = "GTA SanAndreas";
	
	RegisterClassEx(&windowClass);

	window = CreateWindow(windowClass.lpszClassName, "GTA SanAndreas External Menu", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, windowClass.hInstance, nullptr);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	menu = std::make_unique<Menu>(window);

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &pSwapChain, &pDevice, nullptr, &pContext);

	ID3D11Texture2D* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTarget);
	pBackBuffer->Release();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

App::~App()
{
	if (pDevice) {
		pDevice->Release();
		pDevice = nullptr;
	}
	if (pContext) {
		pContext->Release();
		pContext = nullptr;
	}
	if (pRenderTarget) {
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}
	if (pSwapChain) {
		pSwapChain->Release();
		pSwapChain = nullptr;
	}
	
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

int App::init()
{
	MSG message = {};
	
	while (true) {
		while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {

			if (message.message == WM_QUIT)
				return static_cast<int>(message.wParam);

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		doFrame();
	}
}

void App::doFrame()
{
	float color[4] = { 0.1f, 0.1f, 0.2f, 1.0f };
	pContext->OMSetRenderTargets(1, &pRenderTarget, nullptr);
	pContext->ClearRenderTargetView(pRenderTarget, color);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	menu->render();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	pSwapChain->Present(1u, 0u);
}


LRESULT WINAPI windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(window);
	case WM_DESTROY:
		PostQuitMessage(0);
	case WM_NCHITTEST: {
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		ScreenToClient(window, &pt);

		RECT rect;
		GetClientRect(window, &rect);

		const LONG borderWidth = 8;

		if (ImGui::IsAnyItemHovered())
			return HTCLIENT;

		if (pt.y >= 0 && pt.y < 30)
				return HTCAPTION;

		return HTCLIENT;
	}
	}

	return DefWindowProc(window, message, wParam, lParam);
}