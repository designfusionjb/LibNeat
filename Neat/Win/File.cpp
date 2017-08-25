#include "File.h"
#include "Neat\Win\Exception.h"

namespace Neat::Win
{
	File::File()
	{
	}

	File File::Create(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTemplateFile)
	{
		File file;
		file.m_handle = ::CreateFileW(
			lpFileName,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);

		if (!file.m_handle.IsValid())
			throw LastErrorException();

		return file;
	}

	void File::Delete(LPCWSTR lpFileName)
	{
		auto success = ::DeleteFileW(lpFileName);
		if (!success)
			throw LastErrorException();
	}

	ULONGLONG File::GetSize() const
	{
		LARGE_INTEGER size = { 0 };
		auto success = ::GetFileSizeEx(
			m_handle,
			&size);

		if (!success)
			throw LastErrorException();

		return static_cast<ULONGLONG>(size.QuadPart);
	}

	DWORD File::Read(
		LPVOID lpBuffer,
		DWORD nBytes,
		LPOVERLAPPED lpOverlapped) const
	{
		DWORD nRead = 0;
		auto success = ::ReadFile(
			m_handle,
			lpBuffer,
			nBytes,
			&nRead,
			lpOverlapped);

		if (!success)
			throw LastErrorException();
		return nRead;
	}

	void File::Write(
		LPCVOID lpBuffer,
		DWORD nBytes,
		LPOVERLAPPED lpOverlapped)
	{
		DWORD nWritten = 0;
		auto success = ::WriteFile(
			m_handle,
			lpBuffer,
			nBytes,
			&nWritten,
			lpOverlapped);

		if (!success)
			throw LastErrorException();
	}
}
