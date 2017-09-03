#pragma once
#include "Neat\Types.h"
#include "Neat\Win\Handle.h"
#include <Windows.h>

namespace Neat::Win
{
	class FileView
	{
		FileView(byte_t* base, size_t size);
		FileView(const FileView& other) = delete;
		FileView& operator=(const FileView& other) = delete;

	public:
		FileView(FileView&& other);
		FileView& operator=(FileView&& other);
		~FileView();

		void Flush(size_t bytes = 0) const;
		void Unmap();

		byte_t* GetBase();
		const byte_t* GetBase() const;
		size_t GetSize() const;

	private:
		byte_t* m_base = nullptr;
		size_t m_size = 0;

		friend class FileMapping;
	};

	inline byte_t* FileView::GetBase()
	{
		return m_base;
	}

	inline const byte_t* FileView::GetBase() const
	{
		return m_base;
	}

	inline size_t FileView::GetSize() const
	{
		return m_size;
	}

	class FileMapping
	{
		FileMapping(Handle&& handle);

	public:
		FileView MapView(
			DWORD dwDesiredAccess,
			ULONGLONG ullFileOffset,
			SIZE_T dwNumberOfBytesToMap) const;

	private:
		Handle m_handle;

		friend class File;
	};

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

		static void Delete(LPCWSTR lpFileName);

		ULONGLONG GetSize() const;

		DWORD Read(
			LPVOID lpBuffer,
			DWORD nBytes,
			LPOVERLAPPED lpOverlapped = nullptr) const;

		void Write(
			LPCVOID lpBuffer,
			DWORD nBytes,
			LPOVERLAPPED lpOverlapped = nullptr);

		FileMapping CreateMapping(
			DWORD flProtect,
			ULONGLONG ullMaximumSize = 0,
			LPCWSTR lpName = nullptr,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr) const;

	private:
		Handle m_handle;
	};
}

