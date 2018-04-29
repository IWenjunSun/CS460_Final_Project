#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

MODULEINFO GetModuleInfo(char* module);

DWORD PatternScanner(char* module, char* pattern, char* mask);

void insertJMPinstr(BYTE* address, DWORD customFunc, int numBytes);

//insert any new functions here