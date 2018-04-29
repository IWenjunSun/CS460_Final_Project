#include "helpers.h"

/* ASM MACROS HERE */


/* Ammo */
char moduleNameAmmo[] = "server_css.dll";
DWORD jumpBackOffsetAmmo = 0x6;
DWORD jumpBackAddrAmmo = 0x0; //set in LoadHookAmmo
char signatureAmmo[] = "\x89\xAE\x00\x00\x00\x00\x8B\x17\x8B\xCF\xFF\x92\x00\x00\x00\x00\x8B\x07"; //found using SigMaker plugin for OllyDebug
char maskAmmo[] = "xx????xxxxxx????xx"; //found using SigMaker plugin for OllyDebug

__declspec(naked) void UnlimitedAmmoHack() {
	
	//here if we have extra instructions, they would be put here

	//now we jump back to the line after our JMP
	__asm JMP[jumpBackAddrAmmo]
}


