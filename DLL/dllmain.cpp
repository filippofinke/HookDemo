#include <Windows.h>
#include <stdio.h>
#include <iostream>

#define OFFSET_CALL_PRINT 0x103C
typedef void (*Print)();

Print originalPrint = NULL;

VOID Log(const char* fmt, ...) {
	char buf[512];
	va_list arg_list;
	ZeroMemory(buf, sizeof(buf));
	va_start(arg_list, fmt);
	vsnprintf(buf, sizeof(buf), fmt, arg_list);
	va_end(arg_list);
	OutputDebugStringA(buf);
}

static void HookedPrint() {
	std::cout << "You have been hacked :D" << std::endl;

	// Call the original print function.
	originalPrint();
}

VOID Main() {
	Log("DemoHook - HookedPrint() Address: 0x%08x\n", HookedPrint);

	DWORD32 base = (DWORD32)GetModuleHandleA(NULL);
	Log("DemoHook - Process base Address: 0x%08x\n", base);

	DWORD32 address = base + OFFSET_CALL_PRINT;
	Log("DemoHook - Address to patch: 0x%08x\n", address);

	DWORD32 offset = (DWORD32)HookedPrint - address - 5;
	Log("DemoHook - Relative offset from 0x%08x to 0x%08x: 0x%08x\n", address, HookedPrint, offset);

	DWORD old;
	VirtualProtect((LPVOID)address, 5, PAGE_EXECUTE_READWRITE, &old);
	
	DWORD32 originalFunction = (*(DWORD32*)(address + 1) + 5 + address);
	Log("DemoHook - originalFunction Address: 0x%08x\n", originalFunction);
	originalPrint = (Print)originalFunction;

	*(DWORD32*)(address + 1) = offset;
	VirtualProtect((LPVOID)address, 5, old, NULL);

	Log("DemoHook - Hook applied!\n", address);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		Log("DemoHook - Attached to process!");
        Main();
    }

    DisableThreadLibraryCalls(hModule);
    return TRUE;
}

