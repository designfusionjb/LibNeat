#pragma once
#include <stdint.h>

namespace Neat
{
	inline size_t operator""_sz(unsigned long long value)
	{
		return static_cast<size_t>(value);
	}
}