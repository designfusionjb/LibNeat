#include "Neat\Convert.h"

#include <codecvt>

namespace Neat::Convert
{
	//
	// ToUtf8
	//

	Utf8 ToUtf8(const char* value)
	{
		return Utf8(value);
	}

	Utf8 ToUtf8(const wchar_t* value)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
		const auto result = convert.to_bytes(value);
		return Utf8{ result.c_str(), result.size() };
	}

	Utf8 ToUtf8(const Utf8& value)
	{
		return Utf8(value);
	}

	Utf8 ToUtf8(const Utf16& value)
	{
		return ToUtf8(value.operator const wchar_t*());
	}

	Utf8 ToUtf8(const std::string& value)
	{
		return Utf8(value.c_str());
	}

	Utf8 ToUtf8(const std::wstring& value)
	{
		return ToUtf8(value.c_str());
	}

	Utf8 ToUtf8(const int32_t value)
	{
		return Utf8::Format("%i", value);
	}

	Utf8 ToUtf8(const int64_t value)
	{
		return Utf8::Format("%lli", value);
	}

	Utf8 ToUtf8(const uint32_t value)
	{
		return Utf8::Format("%u", value);
	}

	Utf8 ToUtf8(const uint64_t value)
	{
		return Utf8::Format("%llu", value);
	}

	Utf8 ToUtf8(const double value)
	{
		return Utf8::Format("%f", value);
	}

	Utf8 ToUtf8(const IBuffer& value)
	{
		auto nibble = [](byte_t n)
		{
			if (n < 10)
				return '0' + n;
			n -= 10;
			return 'A' + n;
		};

		const auto buffer = value.GetBuffer();
		const auto size = value.GetSize();

		Utf8 result;
		result.Reserve(size * 2);

		char hex[3] = {0};
		for (size_t i = 0; i < size; i++)
		{
			const auto byte = buffer[i];
			hex[0] = nibble(byte >> 4);
			hex[1] = nibble(byte & 0xF);
			result.Append(hex, 3);
		}
		return result;
	}

	//
	// ToUtf16
	//

	Utf16 ToUtf16(const char* value)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
		const auto result = convert.from_bytes(value);
		return Utf16{ result.c_str(), result.size() };
	}

	Utf16 ToUtf16(const wchar_t* value)
	{
		return Utf16(value);
	}

	Utf16 ToUtf16(const Utf8& value)
	{
		return ToUtf16(value.operator const char*());
	}

	Utf16 ToUtf16(const Utf16& value)
	{
		return Utf16(value);
	}

	Utf16 ToUtf16(const std::string& value)
	{
		return ToUtf16(value.c_str());
	}

	Utf16 ToUtf16(const std::wstring& value)
	{
		return Utf16(value.c_str());
	}

	Utf16 ToUtf16(const int32_t value)
	{
		return Utf16::Format(L"%i", value);
	}

	Utf16 ToUtf16(const int64_t value)
	{
		return Utf16::Format(L"%lli", value);
	}

	Utf16 ToUtf16(const uint32_t value)
	{
		return Utf16::Format(L"%u", value);
	}

	Utf16 ToUtf16(const uint64_t value)
	{
		return Utf16::Format(L"%llu", value);
	}

	Utf16 ToUtf16(const double value)
	{
		return Utf16::Format(L"%f", value);
	}

	Utf16 ToUtf16(const IBuffer& value)
	{
		auto nibble = [](byte_t n)
		{
			if (n < 10)
				return L'0' + n;
			n -= 10;
			return L'A' + n;
		};

		const auto buffer = value.GetBuffer();
		const auto size = value.GetSize();

		Utf16 result;
		result.Reserve(size * 2);

		wchar_t hex[3] = { 0 };
		for (size_t i = 0; i < size; i++)
		{
			const auto byte = buffer[i];
			hex[0] = nibble(byte >> 4);
			hex[1] = nibble(byte & 0xF);
			result.Append(hex, 3);
		}
		return result;
	}

	//
	// ToBuffer
	//

	Buffer ToBuffer(const char* value)
	{
		auto nibble = [](char c)
		{
			switch (c)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return static_cast<byte_t>(c - '0');
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				return static_cast<byte_t>(c - 'a' + 10);
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return static_cast<byte_t>(c - 'A' + 10);
			default:
				throw std::runtime_error("Not a hex!");
			}
		};

		const auto size = Utf8::GetLength(value) / 2;

		Buffer buffer(size);
		for (size_t i = 0; i < size; i++)
		{
			byte_t byte = nibble(value[i * 2]);
			byte <<= 4;
			byte |= nibble(value[i * 2 + 1]);
			buffer[i] = byte;
		}

		return buffer;
	}

	Buffer ToBuffer(const wchar_t* value)
	{
		auto nibble = [](wchar_t c)
		{
			switch (c)
			{
			case L'0':
			case L'1':
			case L'2':
			case L'3':
			case L'4':
			case L'5':
			case L'6':
			case L'7':
			case L'8':
			case L'9':
				return static_cast<byte_t>(c - L'0');
			case L'a':
			case L'b':
			case L'c':
			case L'd':
			case L'e':
			case L'f':
				return static_cast<byte_t>(c - L'a' + 10);
			case L'A':
			case L'B':
			case L'C':
			case L'D':
			case L'E':
			case L'F':
				return static_cast<byte_t>(c - L'A' + 10);
			default:
				throw std::runtime_error("Not a hex!");
			}
		};

		const auto size = Utf16::GetLength(value) / 2;

		Buffer buffer(size);
		for (size_t i = 0; i < size; i++)
		{
			byte_t byte = nibble(value[i * 2]);
			byte <<= 4;
			byte |= nibble(value[i * 2 + 1]);
			buffer[i] = byte;
		}

		return buffer;
	}
}
