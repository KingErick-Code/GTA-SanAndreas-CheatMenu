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
        DWORD baseAddress = 0;
        DWORD pointsAddress = 0;

        Address(DWORD offsetToBaseAddress, const std::vector<DWORD>& offsets, Memory& memory)
            : offsetToBaseAddress(offsetToBaseAddress), offsets(offsets) {

            // If no offsets, use the direct address
            if (offsets.empty()) {
                pointsAddress = offsetToBaseAddress;
                return;
            }

            // Read the base address from the game's memory
            if (!ReadProcessMemory(memory.processHandle, (LPVOID)(memory.gameBaseAddress + offsetToBaseAddress), &baseAddress, sizeof(baseAddress), NULL)) {
                MessageBoxA(NULL, "Failed to read base address", "Error", MB_OK);
                return;
            }

            pointsAddress = baseAddress;

            // Resolve the pointer chain
            for (size_t i = 0; i < offsets.size(); i++) {
                if (!ReadProcessMemory(memory.processHandle, (LPVOID)(pointsAddress + offsets[i]), &pointsAddress, sizeof(pointsAddress), NULL)) {
                    MessageBoxA(NULL, "Failed to read memory with offsets", "Error", MB_OK);
                    return;
                }
            }
        }
    };


public:
	Address* Health;
	Address* Money;
    Address* TextBox;
    Address* WantedLevel;

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