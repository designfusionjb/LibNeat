#include "Neat\Win\Path.h"
#include "Neat\Win\Exception.h"
#include "Neat\Win\Environment.h"

#include <Shellapi.h>
#pragma comment(lib, "Shell32.lib")

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace Neat::Win
{
	Path16 Path16::GetCurrent()
	{
		const auto length = ::GetCurrentDirectoryW(0, nullptr);
		Utf16 dir(length - 1);
		const auto success = ::GetCurrentDirectoryW(length, dir);
		if (!success)
			throw LastErrorException();
		return dir;
	}

	Path16 Path16::UrlToWin32(const wchar_t* url)
	{
		DWORD length = MAX_PATH + 1;
		wchar_t win32Path[MAX_PATH + 1] = {0};

		auto hr = ::PathCreateFromUrlW(url, win32Path, &length, NULL);
		if (hr != S_OK)
			throw Win32Exception(hr);

		return Path16(win32Path);
	}

	Path16 Path16::GetFileName(Utf16 cmdLine)
	{
		static Utf16 rundll(L"rundll32.exe");
			
		cmdLine.Trim(L" \t");
		Utf16 lowerCopy = static_cast<const Utf16&>(cmdLine).ToLower();
			
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
		lowerCopy = static_cast<const Utf16&>(cmdLine).ToLower();

		if (cmdLine.BeginsWith(LR"(")"))
		{
			wchar_t** filename = [&cmdLine]()
			{
				int32_t argc;
				return ::CommandLineToArgvW(cmdLine, &argc);
			}();

			if (filename)
			{
				cmdLine = Utf16(filename[0]);
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

	Path16 Path16::NtToWin32(const wchar_t* ntPath, bool kernelRules)
	{
		static const auto systemRoot = Environment::ExpandString(L"%windir%");
		static const auto systemDrive = Environment::ExpandString(L"%SystemDrive%");

		Utf16 path(ntPath);
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

	Path16 Path16::SearchFullPath(const wchar_t* fileName)
	{
		uint32_t length = ::SearchPathW(NULL, fileName, NULL, 0, NULL, NULL);
		if (length > 0)
		{
			Utf16 string(length - 1);
			if (::SearchPathW(NULL, fileName, NULL, length, string, NULL) > 0)
			{
				return Path16(std::move(string));
			}
		}

		return Path16(fileName);
	}

	Path16 Path16::GetFullPath(const wchar_t* path)
	{
		auto relative = ::PathIsRelativeW(path);
		if (relative)
		{
			return SearchFullPath(path);
		}

		return Path16(path);
	}

	std::pair<Utf16, Utf16> Path16::SplitCommandLine(const wchar_t* commandLine)
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
			return std::make_pair(Utf16(first, quote - first), Utf16(second));
		}
		else
		{
			auto space = skipWord(first);
			Utf16 executable(first, space - first);
			const auto expanded = Environment::ExpandString(executable);
			const auto found = ::SearchPathW(NULL, expanded, NULL, 0, NULL, NULL);
			if (found)
			{
				second = skipSpaces(space + 1);
				return std::make_pair(std::move(executable), Utf16(second));
			}
			else
			{
				return std::make_pair(Utf16(first), L"");
			}
		}
	}
}