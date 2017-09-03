#include "File.h"
#include "Neat\Win\Exception.h"

namespace Neat::Win
{
	FileView::FileView(byte_t* base, size_t size)
		: m_base(base)
		, m_size(size)
	{
	}

	FileView::FileView(FileView&& other)
		: m_base(other.m_base)
		, m_size(other.m_size)
	{
		other.m_base = nullptr;
		other.m_size = 0;
	}

	FileView& FileView::operator=(FileView&& other)
	{
		if (this != &other)
		{
			Unmap();
			m_base = other.m_base;
			m_size = other.m_size;
			other.m_base = nullptr;
			other.m_size = 0;
		}
		return *this;
	}

	FileView::~FileView()
	{
		if (m_base)
			::UnmapViewOfFile(m_base);
	}

	void FileView::Flush(size_t bytes) const
	{	
		auto success = ::FlushViewOfFile(m_base, bytes);
		if (!success)
			throw LastErrorException();
	}

	void FileView::Unmap()
	{
		auto success = ::UnmapViewOfFile(m_base);
		if (!success)
			throw LastErrorException();
		m_base = nullptr;
		m_size = 0;
	}

	FileMapping::FileMapping(Handle&& handle)
		: m_handle(std::move(handle))
	{
	}

	FileView FileMapping::MapView(
		DWORD dwDesiredAccess,
		ULONGLONG ullFileOffset,
		SIZE_T dwNumberOfBytesToMap) const
	{
		ULARGE_INTEGER offset;
		offset.QuadPart = ullFileOffset;

		auto base = ::MapViewOfFile(
			m_handle,
			dwDesiredAccess,
			offset.HighPart,
			offset.LowPart,
			dwNumberOfBytesToMap);

		if (!base)
			throw LastErrorException();

		return { static_cast<byte_t*>(base), dwNumberOfBytesToMap };
	}

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

	FileMapping File::CreateMapping(
		DWORD flProtect,
		ULONGLONG ullMaximumSize,
		LPCWSTR lpName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes) const
	{
		ULARGE_INTEGER size;
		size.QuadPart = ullMaximumSize;

		Handle handle = ::CreateFileMappingW(
			m_handle,
			lpSecurityAttributes,
			flProtect,
			size.HighPart,
			size.LowPart,
			lpName);

		if (!handle)
			throw LastErrorException();

		return handle;
	}
}
