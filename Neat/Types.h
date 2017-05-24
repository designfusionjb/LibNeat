#pragma once
#include <stdint.h>

namespace Neat
{
	inline size_t operator""_sz(unsigned long long value)
	{
		return static_cast<size_t>(value);
	}

#ifndef byte_t
	using byte_t = uint8_t;
#endif
}