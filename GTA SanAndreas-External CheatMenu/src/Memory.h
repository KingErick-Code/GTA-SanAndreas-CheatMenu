#pragma once
#include <Windows.h>
#include <cstddef>
#include <vector>


class Memory
{
	HANDLE processHandle = NULL;
	DWORD procID = NULL;
	DWORD gameBaseAddress = NULL;
    HWND gameWindow = FindWindow(NULL, "GTA: San Andreas");

    struct Address {
        DWORD offsetToBaseAddress;
        std::vector<DWORD> offsets;
        DWORD baseAddress = NULL;
        DWORD pointsAddress = NULL;

        Address(DWORD offsetToBaseAddress, const std::vector<DWORD>& offsets, Memory& memory)
            : offsetToBaseAddress(offsetToBaseAddress), offsets(offsets) {

            if (offsets.empty())
            {
                pointsAddress = offsetToBaseAddress;
                return;
            }

            // Step 1: Read the base pointer first
            baseAddress = offsetToBaseAddress;
            if (!ReadProcessMemory(memory.processHandle, (LPCVOID)baseAddress, &pointsAddress, sizeof(pointsAddress), nullptr)) {
                MessageBoxA(NULL, "Failed to read base address", "Error", MB_OK | MB_ICONERROR);
                return;
            }

            for (int i = 0; i < offsets.size() - 1; i++)
            {
                if (!ReadProcessMemory(memory.processHandle, (LPVOID)(pointsAddress + offsets[i]), &pointsAddress, sizeof(pointsAddress), NULL))
                {
                    MessageBoxA(NULL, "Failed to read memory with offsets", "Error", MB_OK);
                    return;
                }
            }

            pointsAddress += offsets.at(offsets.size() - 1);
        }
    };



public:
    Address* Health;
    Address* Armor;
    Address* MaxHealth;
	Address* Money;
    Address* TextBox;
    Address* MenuState;
    Address* WantedLevel;
    Address* PlayerPosX;
    Address* PlayerPosY;
    Address* PlayerPosZ;


public:
	Memory();
	~Memory();

	DWORD GetModuleBaseAddress(const char* lpszModuleName, DWORD processID);
	HANDLE GetProcessHandle() const { return processHandle; }

    template <typename T>
    bool ReadMemory(uintptr_t address, T& outputVal) {
        return ReadProcessMemory(processHandle, (LPCVOID)address, &outputVal, sizeof(T), nullptr);
    }

    template <typename T>
    bool WriteMemory(uintptr_t address, T& inputVal) {
        return WriteProcessMemory(processHandle, (LPVOID)address, &inputVal, sizeof(T), nullptr);
    }

	
	template <typename T>
	void Freeze(DWORD address, const T& value) {
		T currentValue;

		ReadMemory(address, currentValue);

		if (currentValue < value) WriteMemory(address, value);
	}
};