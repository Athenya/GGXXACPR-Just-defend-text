#include <windows.h>
#include "GG.h"

HMODULE exe;
BYTE *target;

void Patch() {
	int enable = 1;
	int Paris = 28;
	int Heaven = 39;
	DWORD oldProtect;
	memcpy(target+0x5ed120, &enable, 4);
	memcpy(target+0x5ed144, &enable, 4); // Enable ♯Reload Grave and Heaven
	VirtualProtect(target+0x2196B8, 20, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(target+0x2196B9, &Paris, 1); 
	memcpy(target+0x2196CA, &Heaven, 1); //Fix rain in ♯Reload Paris and Heaven
	VirtualProtect(target+0x2196B8, 20, oldProtect, &oldProtect);

	return;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		exe = GetModuleHandle(NULL);
		target = (BYTE*)exe;
		Patch();
	}
	return TRUE;
}
