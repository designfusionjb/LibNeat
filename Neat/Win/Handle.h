#pragma once
#include "Neat\Handle.h"

#include <Windows.h>

namespace Neat::Win
{
	namespace Traits
	{
		struct Handle
		{
			static bool IsValid(HANDLE handle);
			static void Finalize(HANDLE handle);
		};

		struct FindHandle
		{
			static bool IsValid(HANDLE handle);
			static void Finalize(HANDLE handle);
		};

		struct ModuleHandle
		{
			static bool IsValid(HMODULE handle);
			static void Finalize(HMODULE handle);
		};

		struct RegistryHandle
		{
			static bool IsValid(HKEY handle);
			static void Finalize(HKEY handle);
		};

		struct ServiceHandle
		{
			static bool IsValid(SC_HANDLE handle);
			static void Finalize(SC_HANDLE handle);
		};
	}

	typedef HandleT<HANDLE, Traits::Handle> Handle;
	typedef HandleT<HANDLE, Traits::FindHandle> FindHandle;
	typedef HandleT<HMODULE, Traits::ModuleHandle> ModuleHandle;
	typedef HandleT<HKEY, Traits::RegistryHandle> RegistryHandle;
	typedef HandleT<SC_HANDLE, Traits::ServiceHandle> ServiceHandle;
}

