#pragma once
#include "Neat\Types.h"
#include "Neat\Utf.h"

#include <utility>

namespace Neat::Win
{
	class Path : public String
	{
		typedef String Base;

	public:
		Path(const wchar_t* path = nullptr);
		Path(String&& path);

		Path(Path&& other);
		Path(const Path& other);

		Path& operator=(Path&& other);
		Path& operator=(const Path& other);

		Path& operator+=(const Path& other);
		Path operator+(const Path& other);

		void Append(const wchar_t* path, size_t length = End);
		void Append(const wchar_t t) = delete;

		void ReplaceName(const String& other);
		void ReplaceFullName(const String& other);
		void ReplaceExtension(const String& other);
		void ReplaceDirectory(const String& other);

		String GetName() const;
		String GetNameWithoutExtension() const;
		String GetFolder() const;

		static Path GetCurrent();
		static Path UrlToWin32(const wchar_t* url);
		static Path GetFileName(String cmdLine);
		static Path NtToWin32(const wchar_t* ntPath, bool kernelRules);
		static Path SearchFullPath(const wchar_t* fileName);
		static Path GetFullPath(const wchar_t* path);
		static std::pair<String, String> SplitCommandLine(const wchar_t* commandLine);

	protected:
		void MoveFrom(Path& other);
		void CopyFrom(const Path& other);
	};
}