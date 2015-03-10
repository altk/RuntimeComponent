#include "pch.h"

BOOL APIENTRY DllMain(HMODULE /* hModule */, DWORD ul_reason_for_call, LPVOID /* lpReserved */)
{
	OutputDebugStringW(L"Hello from DLL.\r\n");
	return TRUE;
}

