#include "helpers.h"
#include "hooks.h"

/* TODO REWORK THIS */

//Setup our infinite ammo by changing an instruction
//You can also ignore the instructions
//Store where we jump back to after our hook

__declspec(naked) void UnlimitedAmmoHack(){
	//we ignore the line because we dont want to change ammo
	

	//here if we have extra instructions, they would be put here

	//now we jump back to the line after our JMP
	__asm JMP[jumpBackAddrAmmo]
}


