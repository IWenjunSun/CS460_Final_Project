#include "helpers.h"

MODULEINFO GetModuleInfo(char* module) {
	MODULEINFO moduleInfo = { 0 };
	HMODULE hModule = GetModuleHandle(module);

	//check for valid module handler
	if (hModule == 0) {
		MessageBox(NULL, "Invalid Module", "Error", MB_OK);
		return moduleInfo;
	}

	//get the information about the loaded module handler (info includes base address and size of module)
	GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));

	return moduleInfo;
}


DWORD PatternScanner(char* module, char* pattern, char* mask) {

	//get the module info for the input dll
	MODULEINFO moduleInfo = GetModuleInfo(module);

	//base address of input dll
	DWORD baseAddr = (DWORD)moduleInfo.lpBaseOfDll;
	//size of input dll
	DWORD size = (DWORD)moduleInfo.SizeOfImage;
	//length of pattern (needed to find a match)
	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++) {
		int matchCount = 0;

		for (DWORD j = 0; j < patternLength; j++) {
			//match is found if mask is '?' or the bytes match
			if ((mask[j] == 'x') && (pattern[j] != *(char*)(baseAddr + i + j)))
				break;

			matchCount++;
		}

		if (matchCount == patternLength)
			return baseAddr + i;

		//if a match is found, return the starting address of the match
	}

	MessageBox(NULL, "No Pattern Match Found :(", "Error", MB_OK);

	return 0;
}

void insertJMPinstr(BYTE* address, DWORD customFunc, int numBytes) {
	unsigned long saveOldPriv;
	DWORD JMPsize;

	//creates read and write privileges to address and saves its old privileges in saveProtect
	VirtualProtect(address, numBytes, PAGE_EXECUTE_READWRITE, &saveOldPriv);

	//The distance we have to jump, not including the new JMP instr
	JMPsize = (DWORD)(customFunc - (DWORD)address - 0x5);

	//add the new JMP instruction opcode at the input addr
	*address = 0xE9; //0xE9 is JMP

	//write the value of the JMP instr
	*((DWORD*)(address + 0x1)) = JMPsize;

	//NOOP any extra bytes
	for (int i = 5; i < numBytes; i++)
		*(address + i) = 0x90; //x90 is NOOP

	//reassign the old privileges back to the input address
	VirtualProtect((LPVOID)(address), numBytes, saveOldPriv, NULL);
}