#pragma once
#include "Neat\Types.h"
#include "Neat\Utf.h"

#include <string>

namespace Neat::Convert
{
	//
	// ToUtf8
	//

	Utf8 ToUtf8(const char* value);
	Utf8 ToUtf8(const wchar_t* value);
	Utf8 ToUtf8(const Utf8& value);
	Utf8 ToUtf8(const Utf16& value);
	Utf8 ToUtf8(const std::string& value);
	Utf8 ToUtf8(const std::wstring& value);

	Utf8 ToUtf8(const int32_t value);
	Utf8 ToUtf8(const int64_t value);
	Utf8 ToUtf8(const uint32_t value);
	Utf8 ToUtf8(const uint64_t value);
	Utf8 ToUtf8(const double value);
	Utf8 ToUtf8(const IBuffer& value);

	//
	// ToUtf16
	//

	Utf16 ToUtf16(const char* value);
	Utf16 ToUtf16(const wchar_t* value);
	Utf16 ToUtf16(const Utf8& value);
	Utf16 ToUtf16(const Utf16& value);
	Utf16 ToUtf16(const std::string& value);
	Utf16 ToUtf16(const std::wstring& value);

	Utf16 ToUtf16(const int32_t value);
	Utf16 ToUtf16(const int64_t value);
	Utf16 ToUtf16(const uint32_t value);
	Utf16 ToUtf16(const uint64_t value);
	Utf16 ToUtf16(const double value);
	Utf16 ToUtf16(const IBuffer& value);

	//
	// ToBuffer
	//

	Buffer ToBuffer(const char* value);
	Buffer ToBuffer(const wchar_t* value);
}

