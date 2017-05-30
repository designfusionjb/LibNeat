#pragma once
#include <stdint.h>

namespace Neat
{
	inline constexpr size_t operator""_sz(unsigned long long value)
	{
		return static_cast<size_t>(value);
	}

	inline constexpr size_t operator""_kB(unsigned long long value)
	{
		return static_cast<size_t>(value * 1024);
	}

	inline constexpr size_t operator""_MB(unsigned long long value)
	{
		return static_cast<size_t>(value * 1024 * 1024);
	}

	inline constexpr size_t operator""_GB(unsigned long long value)
	{
		return static_cast<size_t>(value * 1024 * 1024 * 1024);
	}
}

#ifndef byte_t
using byte_t = uint8_t;
#endif
