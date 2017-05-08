#include "Neat\Win\InternetHandle.h"

namespace Neat::Win::Traits
{
	bool InternetHandle::IsValid(HINTERNET handle)
	{
		return handle != nullptr;
	}

	void InternetHandle::Finalize(HINTERNET handle)
	{
		::InternetCloseHandle(handle);
	}

}