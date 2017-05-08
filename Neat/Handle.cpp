#include "Neat\Handle.h"
#include <stdio.h>

namespace Neat
{
	template <>
	bool HandleTraits<FILE*>::IsValid(FILE* handle)
	{
		return handle != nullptr;
	}

	template <>
	void HandleTraits<FILE*>::Finalize(FILE* handle)
	{
		fclose(handle);
	}
}