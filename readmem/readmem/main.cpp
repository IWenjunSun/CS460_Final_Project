#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#define PSAPI_VERSION 1
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib, "psapi.lib")

using namespace std;
const DWORD player_offset = 0x3D24D4;
const DWORD x_rot_off = 0x326EC0;
const DWORD y_rot_off = 0x326EBC;
const DWORD entity_diff = 0x10;
const DWORD player_x_off = 0x348;
const DWORD player_y_off = 0x34C;
const DWORD player_z_off = 0x350;



HMODULE GetBaseAddressByName(DWORD processId, TCHAR *processName){
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (NULL != hProcess) {
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_32BIT | LIST_MODULES_64BIT)) {
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
			if (!_tcsicmp(processName, szProcessName)) {
				CloseHandle(hProcess);
				return hMod;
			}
		}
	}

	CloseHandle(hProcess);
	return 0;
}

int main() {






	HWND hwnd = FindWindowA(NULL, "Counter-Strike Source");
	if (hwnd == NULL) {
		cout << "Cannot find window" << endl;
		Sleep(3000);
		exit(-1);
	}
	else {
		DWORD procID;
		DWORD size = 100;
		LPTSTR buff;
		GetWindowThreadProcessId(hwnd, &procID);
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
		if (procID == NULL) {
			cout << "Cannot obtain process." << endl;
			Sleep(3000);
			exit(-1);
		}
		else {
			HMODULE addr = GetBaseAddressByName(procID, (TCHAR*)"hl2.exe");
			DWORD engine_offset = (DWORD)addr + 0x1FC00000;
			DWORD server_offset = (DWORD)addr + 0x21C00000;
			DWORD player_addr;
			int num_players;
			float x_rot, y_rot;
			float player_x, player_y, player_z;
			//get number of players, excluding self
			ReadProcessMemory(handle, (LPVOID)(engine_offset + 0x77BB94), &num_players, sizeof(num_players), 0);
			num_players--;
			//main aimbot while loop
			while (1) {
				//get player rotation
				ReadProcessMemory(handle, (LPVOID)(engine_offset+x_rot_off), &x_rot, sizeof(x_rot), 0);
				ReadProcessMemory(handle, (LPVOID)(engine_offset + y_rot_off), &y_rot, sizeof(y_rot), 0);
				//get player position
				ReadProcessMemory(handle, (LPVOID)(server_offset + player_offset), &player_addr, sizeof(player_addr), 0);
				ReadProcessMemory(handle, (LPVOID)(player_addr + player_x_off), &player_x, sizeof(player_x), 0);
				ReadProcessMemory(handle, (LPVOID)(player_addr + player_y_off), &player_y, sizeof(player_y), 0);
				ReadProcessMemory(handle, (LPVOID)(player_addr + player_z_off), &player_z, sizeof(player_z), 0);

				/*for (float i = -89; i < 90; i++) {
					WriteProcessMemory(handle, (LPVOID)(engine_offset + y_rot_off), &i, sizeof(i), 0);
					Sleep(10);
				}*/
			}
		}
	}

	return 0;
}