// NFS Carbon - code workarounds for shaders

#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "includes\injector\injector.hpp"

#define HDROBJ_ADDRESS 0x00B43014

int Init()
{
	injector::WriteMemory<int>(0x00748745, HDROBJ_ADDRESS, true); // make the GaussianBlur5x5 be called from the HDR shader instead so that it doesn't break the bloom!

	return 0;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Init();
	}
	return TRUE;
}

