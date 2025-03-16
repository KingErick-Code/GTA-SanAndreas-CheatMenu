#include "Memory.h"
#include <TlHelp32.h>
#include <tchar.h>

Memory::Memory() {
	// Find the game window
	if (gameWindow == NULL) {
		MessageBoxA(NULL, "Ensure Game is running!", "Couldn't find the game window", MB_OK);
		return;
	}

	// Get the process ID of the game
	GetWindowThreadProcessId(gameWindow, &procID);

	// Open the process
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	if (processHandle == INVALID_HANDLE_VALUE) {
		MessageBoxA(NULL, "Failed to open process", "Error", MB_OK);
		return;
	}

	// Retrieve the base address of the game module
	gameBaseAddress = GetModuleBaseAddress("gta_sa.exe", procID);
	if (!gameBaseAddress) {
		MessageBoxA(NULL, "Failed to get base address", "Error", MB_OK);
	}

	Health = new Address{ 0xB6F5F0, {0x540}, *this };
	MaxHealth = new Address{ 0xB6F5F0, {0x544}, *this };
	Armor = new Address{ 0xB6F5F0 , {0x548}, *this };
	Money = new Address{ 0xB7CE50, {}, *this };
	TextBox = new Address{ 0xBAA7A0, {}, *this };
	WantedLevel = new Address{ 0xBAA420, {}, *this };
	MenuState = new Address{ 0xB7CB49 , {}, *this };
	PlayerPosX = new Address{ 0xB6F5F0, {0x14, 0x30}, *this };
	PlayerPosY = new Address{ 0xB6F5F0, {0x14, 0x34}, *this };
	PlayerPosZ = new Address{ 0xB6F5F0, {0x14, 0x38}, *this };
}

Memory::~Memory() {
	if (processHandle) CloseHandle(processHandle);
	if (Health) delete Health;
	if (MaxHealth) delete MaxHealth;
	if (Armor) delete Armor;
	if (Money) delete Money;
	if (TextBox) delete TextBox;
	if (WantedLevel) delete WantedLevel;
	if (MenuState) delete MenuState;
	if (PlayerPosX) delete PlayerPosX;
	if (PlayerPosY) delete PlayerPosY;
	if (PlayerPosZ) delete PlayerPosZ;
}

DWORD Memory::GetModuleBaseAddress(const char* moduleName, DWORD processID) {
	DWORD baseAddress = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
	if (snapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);
		if (Module32First(snapshot, &moduleEntry)) {
			do {
				if (!_stricmp(moduleEntry.szModule, moduleName)) {
					baseAddress = (DWORD)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(snapshot, &moduleEntry));
		}
		CloseHandle(snapshot);
	}
	return baseAddress;
}
