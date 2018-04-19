#include <Windows.h>
#include "helpers.h"

/* Ammo */
char moduleNameAmmo[] = "server_css.dll";
char fillerBytesAmmo[] = "\x90\x90\x90\x90\x90\x90"; //x90 is NOOP instrution
int fillerCount = 6; //above string has 6 bytes
char signatureAmmo[] = "\x89\xAE\x00\x00\x00\x00\x8B\x17\x8B\xCF\xFF\x92\x00\x00\x00\x00\x8B\x07"; //found using SigMaker plugin for OllyDebug
char maskAmmo[] = "xx????xxxxxx????xx"; //found using SigMaker plugin for OllyDebug


void LoadHooks() {
	//get the address of the DLL
	DWORD address = PatternScanner(moduleNameAmmo, signatureAmmo, maskAmmo);

	//overwrite instruction at found address
	WriteToMemory(address, fillerBytesAmmo, fillerCount);
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpreserver) { //this parameters are neccesary for DLLs

	//following runs when the DLL is attached to a process
	if (fdwReason == DLL_PROCESS_ATTACH) {
		LoadHooks();
	}

	return TRUE;
}

