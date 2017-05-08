#pragma once
#include "Neat\Win\Handle.h"

#include <Windows.h>

namespace Neat::Win
{
	template <class Fn>
	Fn Function(const char* moduleName, const char* functionName)
	{
		ModuleHandle moduleHandle = ::LoadLibraryA(moduleName);
		return (Fn)::GetProcAddress(moduleHandle, functionName);
	}

	template <class Fn>
	Fn Function(const HMODULE moduleHandle, const char* functionName)
	{
		return (Fn)::GetProcAddress(moduleHandle, functionName);
	}
}