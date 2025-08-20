#include <windows.h>
#include "app.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd)
{
	App app;

	app.init();

	return 0;
}