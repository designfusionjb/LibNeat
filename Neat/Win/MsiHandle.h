#pragma once
#include "Neat\Types.h"
#include "Neat\Handle.h"

#include <Windows.h>
#include <Msi.h>
#pragma comment(lib, "Msi.lib")

namespace Neat::Win
{
	namespace Traits
	{
		struct MsiHandle
		{
			static bool IsValid(MSIHANDLE handle);
			static void Finalize(MSIHANDLE handle);
		};
	}
	
	typedef HandleT<MSIHANDLE, Traits::MsiHandle> MsiHandle;
}