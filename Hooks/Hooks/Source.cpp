#include <Windows.h>
#include "helpers.h"
#include "hooks.h"



void LoadAmmoHook() {
	//get the address of the DLL
	DWORD address = PatternScanner(moduleNameAmmo, signatureAmmo, maskAmmo);

	//no need to offset address since our mask starts at the memory location we want
	//address+=0 

	jumpBackAddrAmmo = address + jumpBackOffsetAmmo; //used in assembly to return
	insertJMPinstr((BYTE*)address, (DWORD)UnlimitedAmmoHack, jumpBackOffsetAmmo);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpreserver) { //this parameters are neccesary for DLLs

	//following runs when the DLL is attached to a process
	if (fdwReason == DLL_PROCESS_ATTACH) {
		LoadAmmoHook();
		//create threads here if we want some KeyPress to trigger something
	}

	return TRUE;
}