#pragma once

#include <d3d9.h>
#include "Memory.h"

namespace GUI {
    extern LPDIRECT3D9 d3d;
    extern LPDIRECT3DDEVICE9 d3dDevice;
    extern bool deviceLost;
    extern UINT resizeWidth, resizeHeight;
    extern D3DPRESENT_PARAMETERS d3dpp;


    bool Initialize(HWND hWnd);
    void Shutdown();
    void Render();
    void ResetDevice();
    void ProcessInput();
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
