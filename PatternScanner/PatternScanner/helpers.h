#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>


MODULEINFO GetModuleInfo(char* module);

void WriteToMemory(DWORD address, char* value, int numBytes);

DWORD PatternScanner(char* module, char* pattern, char* mask);

