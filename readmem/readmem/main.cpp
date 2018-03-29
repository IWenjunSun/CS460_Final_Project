#include <iostream>
#include <Windows.h>
#include <Psapi.h>

using namespace std;
DWORD  base = 0x0C301B7C;
DWORD  hello = 0 ;
//DWORD* ptr = base;
DWORD hp = 0x11069bc;
float* ptrhp;
float myhp;
int myammo;
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
			while (1) {
				ReadProcessMemory(handle, (LPVOID)(0x0C301B7C + 0x1e0), &myhp, sizeof(myhp), 0);
				cout << myhp << endl;
				Sleep(100);
			}
		}
	}

	return 0;
}