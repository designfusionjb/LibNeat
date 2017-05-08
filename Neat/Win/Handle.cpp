#include "Neat\Win\Handle.h"

namespace Neat::Win::Traits
{
	bool Handle::IsValid(HANDLE handle)
	{
		return handle != INVALID_HANDLE_VALUE;
	}

	void Handle::Finalize(HANDLE handle)
	{
		::CloseHandle(handle);
	}

	bool FindHandle::IsValid(HANDLE handle)
	{
		return handle != INVALID_HANDLE_VALUE;
	}

	void FindHandle::Finalize(HANDLE handle)
	{
		::FindClose(handle);
	}

	bool ModuleHandle::IsValid(HMODULE handle)
	{
		return handle != NULL;
	}

	void ModuleHandle::Finalize(HMODULE handle)
	{
		::FreeLibrary(handle);
	}

	bool RegistryHandle::IsValid(HKEY handle)
	{
		return handle != nullptr;
	}

	void RegistryHandle::Finalize(HKEY handle)
	{
		::RegCloseKey(handle);
	}

	bool ServiceHandle::IsValid(SC_HANDLE handle)
	{
		return handle != nullptr;
	}

	void ServiceHandle::Finalize(SC_HANDLE handle)
	{
		::CloseServiceHandle(handle);
	}
}