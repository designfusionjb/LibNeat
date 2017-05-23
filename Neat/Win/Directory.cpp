#include "Directory.h"
#include "Neat\Win\Exception.h"

namespace Neat::Win
{
	void Directory::Create(
		LPCWSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		auto success = ::CreateDirectoryW(
			lpPathName,
			lpSecurityAttributes);

		if (success)
			return;

		auto error = ::GetLastError();
		if (ERROR_ALREADY_EXISTS == error)
			return;

		throw Win32Exception(error);
	}
}
