#pragma once
#include "Neat\Types.h"
#include "Neat\Utf.h"
#include "Neat\Win\Handle.h"

namespace Neat::Win
{
	class Environment
	{
	public:
		static String ExpandString(const wchar_t* string, const Handle& token = Handle());
		static String GetVariable(const wchar_t* name);
	};
}
