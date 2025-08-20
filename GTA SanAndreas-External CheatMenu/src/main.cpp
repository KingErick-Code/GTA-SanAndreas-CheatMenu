#include "gui.h"

#ifdef _DEBUG
int main(int, char**) {
#else
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif // _DEBUG

	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, GUI::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"GTA San Andreas Cheat Menu", nullptr };
	::RegisterClassExW(&wc);
	HWND hWnd = ::CreateWindowW(wc.lpszClassName, L"GTA SanAndreas CheatMenu", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);

	if (!GUI::Initialize(hWnd)) {
		GUI::Shutdown();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return -1;
	}

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

	bool done = false;
	while (!done) {
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessageA(&msg);
			if (msg.message == WM_QUIT) done = true;
		}
		if (done) break;

		GUI::ProcessInput();
		GUI::Render();
	}

	GUI::Shutdown();
	::DestroyWindow(hWnd);
	::UnregisterClassW(wc.lpszMenuName, wc.hInstance);

#ifdef _CONSOLE
	return 0;
#else
	return 0;
#endif // _CONSOLE
}
