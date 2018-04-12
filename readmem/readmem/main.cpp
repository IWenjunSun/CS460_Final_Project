#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#define PSAPI_VERSION 1
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib, "psapi.lib")

using namespace std;
DWORD base = 0x905a4d;
DWORD player_offset = 0x11069bc;
DWORD hp_offset = 0x1e0;
DWORD base_addr;
//DWORD* ptr = base;
DWORD hp = 0x11069bc;
float myhp = 100;


int myammo;




HMODULE GetBaseAddressByName(DWORD processId, TCHAR *processName)
{
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

	HWND hwnd = FindWindowA(NULL, "Counter-Strike");
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
			HMODULE addr = GetBaseAddressByName(procID, (TCHAR*)"hl.exe");
			while (1) {
				ReadProcessMemory(handle, (LPVOID)((DWORD)addr + 0x11069bc), &base_addr, sizeof(base_addr), 0);
				WriteProcessMemory(handle, (LPVOID)(base_addr + 0x1e0), &myhp, sizeof(myhp), 0);
				cout << base_addr << endl;
				Sleep(100);
			}
		}
	}

	return 0;
}