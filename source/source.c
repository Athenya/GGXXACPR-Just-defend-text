#include <windows.h>
#include "GG.h"

HMODULE exe;
BYTE *target;
BYTE *patchLocation;
BYTE *Tilbage;
BYTE *Kaldet;
CHAR *Lige  = "JUST!";



void Justdefend() { 
	__asm
	{
		push esi // Save ESI for when we return to normal code  
		push eax // Save EAX for when we return to normal code
		mov esi, offset Lige+4 //Load the location of the text into ESI
		cmp bl, 00 // Check if player 1
		je P1 // Jmp if above is true
		cmp bl, 0x30 // check if player 2
		je P2 // Jmp if above is true
		jne end; //Skip to the end if somehow both checks don't pass

		P1:
		xor eax, eax //Set EAX to 0 so that the message appears on the left
			jmp Kald;

		P2:
		mov eax, 1 //Set EAX to 1 so that the message appears on the right
			jmp Kald;

		Kald:
		call Kaldet // Call code that renders the text

		end:
		pop eax // restore old EAX value 
		pop esi // restore old ESI value
		mov[edi], ax //resume normal code that was replaced when making the JMP here
		pop edi
		mov esi, [esi+0x2C]
		jmp Tilbage // Return to the instruction
	};
}

void Patch() {
	DWORD oldProtect;
	char jump[7] = {0xE9}; // Write the JMP instruction
	*(DWORD*)(jump+1) = (DWORD)Justdefend - ((DWORD)patchLocation); // Write location for the JMP
	jump[5] = 0x66; // write nop
	jump[6] = 0x90; // write nop
	VirtualProtect(patchLocation,7,PAGE_EXECUTE_READWRITE,&oldProtect);
	memcpy(patchLocation,jump,7);
	VirtualProtect(patchLocation,7,oldProtect,&oldProtect);
	return;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		exe = GetModuleHandle(NULL);
		target = (BYTE*)exe;
		patchLocation = target+0x12602D;
		Tilbage = target+0x126032;
		Kaldet = target+0x1E8F50;
		Patch();
	}
	return TRUE;
}
