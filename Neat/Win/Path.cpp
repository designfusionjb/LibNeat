#include "Neat\Win\Path.h"
#include "Neat\Win\Exception.h"
#include "Neat\Win\Environment.h"

#include <Shellapi.h>
#pragma comment(lib, "Shell32.lib")

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace Neat::Win
{
	Path::Path(const wchar_t* path) :
		Base(path)
	{
	}

	Path::Path(String&& path) :
		Base(std::move(path))
	{
	}

	Path::Path(Path&& other)
	{
		MoveFrom(other);
	}

	Path::Path(const Path& other)
	{
		CopyFrom(other);
	}

	Path& Path::operator=(Path&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Path& Path::operator=(const Path& other)
	{
		CopyFrom(other);
		return *this;
	}

	Path& Path::operator+=(const Path& other)
	{
		Append(other);
		return *this;
	}

	Path Path::operator+(const Path& other) const
	{
		Path path(*this);
		path.Append(other);
		return path;
	}

	void Path::Append(const wchar_t* path, size_t length)
	{
		if (String::End == length)
			length = String::GetLength(path);

		if (0 == length)
			return;

		if (IsEmpty())
		{
			Base::Append(path, length);
			return;
		}

		if (!EndsWith(L"\\"))
		{
			if (path[(size_t)0] != L'\\')
				Base::Append('\\');

			Base::Append(path, length);
		}
		else
		{
			if (path[(size_t)0] == L'\\')
				Base::Append(path + 1, length - 1);
			else
				Base::Append(path, length);
		}
	}

	void Path::ReplaceName(const String& name)
	{
		auto slash = FindLast(L"\\");
		auto dot = FindLast(L".");
			
		if (String::End != slash)
		{
			if (String::End != dot)
				Replace(++slash, dot, name);
			else
				Replace(++slash, name);
		}
		else
		{
			if (String::End != dot)
				Replace(0, dot, name);
			else
				static_cast<String&>(*this) = name;
		}
	}

	void Path::ReplaceFullName(const String& fullName)
	{
		auto slash = FindLast(L"\\");
		if (String::End != slash)
			Replace(++slash, fullName);
		else
			static_cast<String&>(*this) = fullName;
	}

	void Path::ReplaceExtension(const String& ext)
	{
		auto dot = FindLast(L".");
		if (String::End != dot)
		{
			Replace(++dot, ext);
		}
		else
		{
			static_cast<String&>(*this) += L".";
			static_cast<String&>(*this) += ext;
		}
	}

	void Path::ReplaceDirectory(const String& dir)
	{
		auto slash = FindLast(L"\\");
		if (String::End != slash)
		{
			Replace(0, slash, dir);
		}
		else
		{
			auto temp = *this;
			static_cast<String&>(*this) = dir;
			Append(temp);
		}
	}

	String Path::GetName() const
	{
		auto slash = FindLast(L"\\");
		if (String::End != slash)
			return Substring(++slash);
		return *this;
	}

	String Path::GetNameWithoutExtension() const
	{
		auto dot = FindLast(L".");
		auto slash = FindLast(L"\\");
		if (String::End != slash)
		{
			slash++;
			if (String::End != dot && dot >= slash)
			{
				auto size = dot - slash;
				return Substring(slash, size);
			}
			else
			{
				return Substring(slash);
			}
		}
		else if (String::End != dot)
		{
			return Substring(0, dot);
		}
		return *this;
	}

	String Path::GetFolder() const
	{
		auto slash = FindLast(L"\\");
		if (0 == slash)
			return *this;

		if (String::End != slash)
		{
			if (m_buffer[slash - 1] == L':')
				return Substring(0, slash + 1);
			return Substring(0, slash);
		}
		return L"";
	}

	Path Path::GetCurrent()
	{
		const auto length = ::GetCurrentDirectoryW(0, nullptr);
		String dir(length - 1);
		const auto success = ::GetCurrentDirectoryW(length, dir);
		if (!success)
			throw LastErrorException();
		return dir;
	}

	Path Path::UrlToWin32(const wchar_t* url)
	{
		DWORD length = MAX_PATH + 1;
		wchar_t win32Path[MAX_PATH + 1] = {0};

		auto hr = ::PathCreateFromUrlW(url, win32Path, &length, NULL);
		if (hr != S_OK)
			throw Win32Exception(hr);

		return Path(win32Path);
	}

	Path Path::GetFileName(String cmdLine)
	{
		static String rundll(L"rundll32.exe");
			
		cmdLine.Trim(L" \t");
		String lowerCopy = static_cast<const String&>(cmdLine).ToLower();
			
		auto dll = lowerCopy.Find(rundll);
		if (-1 != dll)
		{
			auto from = dll + rundll.GetLength() + 1;
			auto comma = cmdLine.Find(L",");
			if (-1 != comma)
			{
				auto count = comma - from;
				cmdLine = cmdLine.Substring(from, count);
			}
			else
			{
				cmdLine = cmdLine.Substring(from);
			}
		}

		cmdLine.Trim(L" \t");
		lowerCopy = static_cast<const String&>(cmdLine).ToLower();

		if (cmdLine.BeginsWith(LR"(")"))
		{
			wchar_t** filename = [&cmdLine]()
			{
				int32_t argc;
				return ::CommandLineToArgvW(cmdLine, &argc);
			}();

			if (filename)
			{
				cmdLine = String(filename[0]);
				::LocalFree(filename);
			}
		}

		auto comma = cmdLine.FindLast(L",");
		if (-1 != comma)
		{
			cmdLine = cmdLine.Substring(0, comma);
		}

		auto slashes = cmdLine.Find(LR"(\\)");
		while (-1 != slashes && 0 != slashes)
		{
			cmdLine.Replace(slashes, slashes + 2, LR"(\)");
			slashes = cmdLine.Find(LR"(\\)");
		}

		auto exe = lowerCopy.Find(L".exe ");
		if (-1 != exe)
		{
			cmdLine = cmdLine.Substring(0, exe + 4);
		}

		if (cmdLine.BeginsWith(LR"(\??\)"))
		{
			cmdLine = cmdLine.Substring(4);
		}

		return cmdLine;
	}

	Path Path::NtToWin32(const wchar_t* ntPath, bool kernelRules)
	{
		static const auto systemRoot = Environment::ExpandString(L"%windir%");
		static const auto systemDrive = Environment::ExpandString(L"%SystemDrive%");

		String path(ntPath);
		path.Replace(L"\\SystemRoot", systemRoot);

		if (path.BeginsWith(L"\\??\\"))
		{
			path = path.Substring(4);
		}

		if (kernelRules)
		{
			if (path.BeginsWith(L"\\"))
			{
				path = systemDrive + path;
			}

			auto slash = path.Find(L"\\");
			if (-1 == slash)
			{
				path = systemRoot + L"\\System32\\Drivers\\" + path;
			}
		}

		return path;
	}

	Path Path::SearchFullPath(const wchar_t* fileName)
	{
		uint32_t length = ::SearchPathW(NULL, fileName, NULL, 0, NULL, NULL);
		if (length > 0)
		{
			String string(length - 1);
			if (::SearchPathW(NULL, fileName, NULL, length, string, NULL) > 0)
			{
				return Path(std::move(string));
			}
		}

		return Path(fileName);
	}

	Path Path::GetFullPath(const wchar_t* path)
	{
		auto relative = ::PathIsRelativeW(path);
		if (relative)
		{
			return SearchFullPath(path);
		}

		return Path(path);
	}

	std::pair<String, String> Path::SplitCommandLine(const wchar_t* commandLine)
	{
		if (nullptr == commandLine || L'\0' == *commandLine)
			throw std::runtime_error("Bad command line");

		auto skipSpaces = [](const wchar_t* string)
		{
			while (true)
			{
				if (L' ' == *string)
					string++;
				else
					return string;
			}
		};

		auto skipQuoted = [](const wchar_t* string)
		{
			while (true)
			{
				if (L'\"' == *string || L'\0' == *string)
					return string;
				else
					string++;
			}
		};

		auto skipWord = [](const wchar_t* string)
		{
			while (true)
			{
				if (L' ' == *string || L'\0' == *string)
					return string;
				else
					string++;
			}
		};

		commandLine = skipSpaces(commandLine);
		auto first = commandLine;
		auto second = L"";

		if (L'\"' == *commandLine)
		{
			first++;
			auto quote = skipQuoted(first);
			second = skipSpaces(quote + 1);
			return std::make_pair(String(first, quote - first), String(second));
		}
		else
		{
			auto space = skipWord(first);
			String executable(first, space - first);
			const auto expanded = Environment::ExpandString(executable);
			const auto found = ::SearchPathW(NULL, expanded, NULL, 0, NULL, NULL);
			if (found)
			{
				second = skipSpaces(space + 1);
				return std::make_pair(std::move(executable), String(second));
			}
			else
			{
				return std::make_pair(String(first), L"");
			}
		}
	}

	void Path::MoveFrom(Path& other)
	{
		if (this != &other)
		{
			Base::MoveFrom(other);
		}
	}

	void Path::CopyFrom(const Path& other)
	{
		if (this != &other)
		{
			Base::CopyFrom(other);
		}
	}
}