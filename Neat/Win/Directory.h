#pragma once
#include <Windows.h>

namespace Neat::Win
{
	class Directory
	{
	public:
		static void Create(
			LPCWSTR lpPathName,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr);
	};
}

