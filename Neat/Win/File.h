#pragma once
#include "Neat\Win\Handle.h"
#include <Windows.h>

namespace Neat::Win
{
	class File
	{
	public:
		File();

		static File Create(
			LPCWSTR lpFileName,
			DWORD dwDesiredAccess,
			DWORD dwShareMode,
			DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr,
			HANDLE hTemplateFile = nullptr);

		void Write(
			LPCVOID lpBuffer,
			DWORD nBytes,
			LPOVERLAPPED lpOverlapped = nullptr);

	private:
		Handle m_handle;
	};
}

