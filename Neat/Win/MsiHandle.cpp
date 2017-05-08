#include "Neat\Win\MsiHandle.h"

namespace Neat::Win::Traits
{
	bool MsiHandle::IsValid(MSIHANDLE handle)
	{
		return handle != 0;
	}

	void MsiHandle::Finalize(MSIHANDLE handle)
	{
		::MsiCloseHandle(handle);
	}

}