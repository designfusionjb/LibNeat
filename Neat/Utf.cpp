#include "Neat\Utf.h"
#include "Neat\Exception.h"

#include <locale>

#include <mbstring.h>
#include <stdarg.h>
#include <stdio.h>

namespace Neat
{
	//
	// UTF-8 string traits
	//

	size_t Utf8Traits::GetLength(const char* string)
	{
		if (nullptr == string)
			return 0;

		return _mbstrlen(string);
	}

	void Utf8Traits::Copy(char* buffer, size_t capacity, const char* source)
	{
		_mbscpy_s((byte_t*)buffer, capacity, (byte_t*)source);
	}

	void Utf8Traits::Copy(char* buffer, size_t capacity, const char* source, size_t length)
	{
		_mbsnbcpy_s((byte_t*)buffer, capacity, (byte_t*)source, length);
	}

	size_t Utf8Traits::FormatCount(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		const auto result = _vscprintf(format, argList);
		if (-1 == result) // -1 means invalid params
			return 0;
		return static_cast<size_t>(result);
	}

	size_t Utf8Traits::Format(char* buffer, size_t size, const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		const auto result = vsprintf_s(buffer, size, format, argList);
		if (-1 == result) // -1 means invalid params
			return 0;
		return static_cast<size_t>(result);
	}

	void Utf8Traits::ToLower(char* string, size_t size)
	{
		std::locale loc("");
		size_t pos = 0;
		while (pos < size && string[pos])
		{
			string[pos] = std::tolower(string[pos], loc);
			pos++;
		}
	}

	void Utf8Traits::ToUpper(char* string, size_t size)
	{
		std::locale loc("");
		size_t pos = 0;
		while (pos < size && string[pos])
		{
			string[pos] = std::toupper(string[pos], loc);
			pos++;
		}
	}

	char Utf8Traits::ToLower(const char c)
	{
		return std::tolower(c, std::locale::classic());
	}

	char Utf8Traits::ToUpper(const char c)
	{
		return std::toupper(c, std::locale::classic());
	}

	int32_t Utf8Traits::Compare(const char* left, const char* right)
	{
		return _mbscmp((const byte_t*)left, (const byte_t*)right);
	}

	int32_t Utf8Traits::Compare(const char* left, const char* right, size_t length)
	{
		return _mbsncmp((const byte_t*)left, (const byte_t*)right, length);
	}

	const char* Utf8Traits::Find(const char* string, const char what)
	{
		return (const char*)_mbschr((const byte_t*)string, what);
	}

	const char* Utf8Traits::Find(const char* string, const char* what)
	{
		return (const char*)_mbsstr((const byte_t*)string, (const byte_t*)what);
	}

	const char* Utf8Traits::FindLast(const char* string, const char what)
	{
		return (const char*)_mbsrchr((const byte_t*)string, what);
	}

	//
	// UTF-16 string traits
	//

	size_t Utf16Traits::GetLength(const wchar_t* string)
	{
		if (nullptr == string)
			return 0;
		return wcslen(string);
	}

	void Utf16Traits::Copy(wchar_t* buffer, size_t capacity, const wchar_t* source)
	{
		wcscpy_s(buffer, capacity, source);
	}

	void Utf16Traits::Copy(wchar_t* buffer, size_t capacity, const wchar_t* source, size_t length)
	{
		wcsncpy_s(buffer, capacity, source, length);
	}

	size_t Utf16Traits::FormatCount(const wchar_t* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		return _vscwprintf(format, argList);
	}

	size_t Utf16Traits::Format(wchar_t* buffer, size_t size, const wchar_t* format, ...)
	{
		va_list argList;
		va_start(argList, format);

		return vswprintf_s(buffer, size / sizeof(wchar_t), format, argList);
	}

	void Utf16Traits::ToLower(wchar_t* string, size_t size)
	{
		std::locale loc("");
		size_t pos = 0;
		while (pos < size && string[pos])
		{
			string[pos] = std::tolower(string[pos], loc);
			pos++;
		}
	}

	void Utf16Traits::ToUpper(wchar_t* string, size_t size)
	{
		std::locale loc("");
		size_t pos = 0;
		while (pos < size && string[pos])
		{
			string[pos] = std::toupper(string[pos], loc);
			pos++;
		}
	}

	wchar_t Utf16Traits::ToLower(const wchar_t w)
	{
		return std::tolower(w, std::locale::classic());
	}

	wchar_t Utf16Traits::ToUpper(const wchar_t w)
	{
		return std::toupper(w, std::locale::classic());
	}

	int32_t Utf16Traits::Compare(const wchar_t* left, const wchar_t* right)
	{
		return wcscmp(left, right);
	}

	int32_t Utf16Traits::Compare(const wchar_t* left, const wchar_t* right, size_t length)
	{
		return wcsncmp(left, right, length);
	}

	const wchar_t* Utf16Traits::Find(const wchar_t* string, const wchar_t what)
	{
		return wcschr(string, what);
	}

	const wchar_t* Utf16Traits::Find(const wchar_t* string, const wchar_t* what)
	{
		return wcsstr(string, what);
	}

	const wchar_t* Utf16Traits::FindLast(const wchar_t* string, const wchar_t what)
	{
		return wcsrchr(string, what);
	}
}