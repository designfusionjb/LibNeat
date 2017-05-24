#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	inline auto ToString(const int64_t& value)
	{
		wchar_t buffer[32];
		swprintf_s(buffer, L"%I64d", value);
		return std::wstring(buffer);
	}
}

