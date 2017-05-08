#pragma once
#include "Neat\Types.h"
#include "Neat\Handle.h"

#include <Windows.h>
#include <Wininet.h>

namespace Neat::Win
{
	namespace Traits
	{
		struct InternetHandle
		{
			static bool IsValid(HINTERNET handle);
			static void Finalize(HINTERNET handle);
		};
	}
	
	typedef HandleT<HINTERNET, Traits::InternetHandle> InternetHandle;
}