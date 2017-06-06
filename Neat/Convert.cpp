#include "Neat\Convert.h"

#include <codecvt>

namespace Neat::Convert
{
	//
	// ToHex
	//

	char ToHex(byte_t value, Base base)
	{
		const auto hex = [=]()
		{
			switch (base)
			{
			case Base::Hex:
				return 'a';
			case Base::HexUp:
				return 'A';
			default:
				throw std::runtime_error("Not implemented");
			}
		}();
		if (value < 10)
			return '0' + value;
		value -= 10;
		return hex + value;
	}

	void ToHex(char* buffer, byte_t value, Base base)
	{
		buffer[0] = ToHex(value >> 4, base);
		buffer[1] = ToHex(value & 0xF, base);
	}

	void ToHex(wchar_t* buffer, byte_t value, Base base)
	{
		buffer[0] = ToHex(value >> 4, base);
		buffer[1] = ToHex(value & 0xF, base);
	}

	//
	// ToNibble
	//

	byte_t ToNibble(char c)
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
	}

	byte_t ToNibble(wchar_t c)
	{
		return ToNibble(static_cast<char>(c));
	}

	//
	// ToByte
	//

	byte_t ToByte(char hi, char lo)
	{
		byte_t byte = ToNibble(hi);
		byte <<= 4;
		byte |= ToNibble(lo);
		return byte;
	}

	byte_t ToByte(wchar_t hi, wchar_t lo)
	{
		byte_t byte = ToNibble(static_cast<char>(hi));
		byte <<= 4;
		byte |= ToNibble(static_cast<char>(lo));
		return byte;
	}

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

	Utf8 ToUtf8(const uint8_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf8::Format("%u", value);
		}

		Utf8 result;
		result.Reserve(2);
		result[0] = ToHex(value >> 4, base);
		result[1] = ToHex(value & 0xF, base);
		return result;
	}

	Utf8 ToUtf8(const uint16_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf8::Format("%hu", value);

		case Base::Hex:
			return Utf8::Format("%04hx", value);

		case Base::HexUp:
			return Utf8::Format("%04hX", value);

		default:
			throw std::runtime_error("Not implemented");
		}
	}

	Utf8 ToUtf8(const uint32_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf8::Format("%u", value);

		case Base::Hex:
			return Utf8::Format("%08x", value);

		case Base::HexUp:
			return Utf8::Format("%08X", value);

		default:
			throw std::runtime_error("Not implemented");
		}
	}

	Utf8 ToUtf8(const uint64_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf8::Format("%llu", value);

		case Base::Hex:
			return Utf8::Format("%016llx", value);

		case Base::HexUp:
			return Utf8::Format("%016llX", value);

		default:
			throw std::runtime_error("Not implemented");
		}
	}

	Utf8 ToUtf8(const double value)
	{
		return Utf8::Format("%f", value);
	}

	Utf8 ToUtf8(const IBuffer& value, Base base)
	{
		const auto buffer = value.GetBuffer();
		const auto size = value.GetSize();

		Utf8 result;
		result.Reserve(size * 2);
		for (size_t i = 0; i < size; i++)
		{
			const auto byte = buffer[i];
			result[i * 2] = ToHex(byte >> 4, base);
			result[i * 2 + 1] = ToHex(byte & 0xF, base);
		}
		return result;
	}

	Utf8 ToUtf8(const Uuid& value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			throw std::runtime_error("Not implemented");
		}

		Utf8 result;
		result.Reserve(Uuid::LengthInHex());

		{
			const auto& data1 = value.GetData1();
			result.Append(ToUtf8(data1, base));
			result.Append('-');
		}
		{
			const auto& data2 = value.GetData2();
			result.Append(ToUtf8(data2, base));
			result.Append('-');
		}
		{
			const auto& data3 = value.GetData3();
			result.Append(ToUtf8(data3, base));
			result.Append('-');
		}
		{
			size_t i = 19;
			const auto& data4 = value.GetData4();
			for (auto byte : data4)
			{
				ToHex(result + i, byte, base);
				i += 2;
			}
			result[i] = '-';
		}
		{
			size_t i = 24;
			const auto& data5 = value.GetData5();
			for (auto byte : data5)
			{
				ToHex(result + i, byte, base);
				i += 2;
			}
			result[i] = '\0';
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

	Utf16 ToUtf16(const uint8_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf16::Format(L"%u", value);
		}

		Utf16 result;
		result.Reserve(2);
		result[0] = ToHex(value >> 4, base);
		result[1] = ToHex(value & 0xF, base);
		return result;
	}
	
	Utf16 ToUtf16(const uint16_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf16::Format(L"%hu", value);

		case Base::Hex:
			return Utf16::Format(L"%04hx", value);

		case Base::HexUp:
			return Utf16::Format(L"%04hX", value);

		default:
			throw std::runtime_error("Not implemented");
		}
	}

	Utf16 ToUtf16(const uint32_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf16::Format(L"%u", value);

		case Base::Hex:
			return Utf16::Format(L"%08x", value);

		case Base::HexUp:
			return Utf16::Format(L"%08X", value);

		default:
			throw std::runtime_error("Not implemented");
		}
	}

	Utf16 ToUtf16(const uint64_t value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			return Utf16::Format(L"%llu", value);

		case Base::Hex:
			return Utf16::Format(L"%016llx", value);

		case Base::HexUp:
			return Utf16::Format(L"%016llX", value);

		default:
			throw std::runtime_error("Not implemented");
		}
	}

	Utf16 ToUtf16(const double value)
	{
		return Utf16::Format(L"%f", value);
	}

	Utf16 ToUtf16(const IBuffer& value, Base base)
	{
		const auto buffer = value.GetBuffer();
		const auto size = value.GetSize();

		Utf16 result;
		result.Reserve(size * 2);
		for (size_t i = 0; i < size; i++)
		{
			const auto byte = buffer[i];
			result[i * 2] = ToHex(byte >> 4, base);
			result[i * 2 + 1] = ToHex(byte & 0xF, base);
		}
		return result;
	}

	Utf16 ToUtf16(const Uuid& value, Base base)
	{
		switch (base)
		{
		case Base::Dec:
			throw std::runtime_error("Not implemented");
		}

		Utf16 result;
		result.Reserve(Uuid::LengthInHex());

		{
			const auto& data1 = value.GetData1();
			result.Append(ToUtf16(data1, base));
			result.Append('-');
		}
		{
			const auto& data2 = value.GetData2();
			result.Append(ToUtf16(data2, base));
			result.Append('-');
		}
		{
			const auto& data3 = value.GetData3();
			result.Append(ToUtf16(data3, base));
			result.Append('-');
		}
		{
			size_t i = 19;
			const auto& data4 = value.GetData4();
			for (auto byte : data4)
			{
				ToHex(result + i, byte, base);
				i += 2;
			}
			result[i] = '-';
		}
		{
			size_t i = 24;
			const auto& data5 = value.GetData5();
			for (auto byte : data5)
			{
				ToHex(result + i, byte, base);
				i += 2;
			}
			result[i] = '\0';
		}

		return result;
	}

	//
	// ToBuffer
	//

	template <typename T>
	Buffer ToBufferT(const T* value)
	{
		const auto size = StringT<T>::GetLength(value) / 2;

		Buffer buffer(size);
		for (size_t i = 0; i < size; i++)
			buffer[i] = ToByte(value[i * 2], value[i * 2 + 1]);

		return buffer;
	}

	Buffer ToBuffer(const char* value)
	{
		return ToBufferT(value);
	}

	Buffer ToBuffer(const wchar_t* value)
	{
		return ToBufferT(value);
	}

	//
	// ToUuid
	//

	template <typename T>
	Uuid ToUuidT(const T* value)
	{
		if (!value)
			throw std::exception();

		const auto length = StringT<T>::GetLength(value);
		if (length < Uuid::LengthInHex())
			throw std::exception();

		Uuid uuid;
		{
			auto& data1 = uuid.GetData1();
			for (auto j = 0; j < sizeof(data1); j++)
			{
				data1 <<= 4;
				data1 |= ToNibble(*value++);
				data1 <<= 4;
				data1 |= ToNibble(*value++);
			}
		}
		value++; // skip '-'
		{
			auto& data2 = uuid.GetData2();
			for (auto j = 0; j < sizeof(data2); j++)
			{
				data2 <<= 4;
				data2 |= ToNibble(*value++);
				data2 <<= 4;
				data2 |= ToNibble(*value++);
			}
		}
		value++; // skip '-'
		{
			auto& data3 = uuid.GetData3();
			for (auto j = 0; j < sizeof(data3); j++)
			{
				data3 <<= 4;
				data3 |= ToNibble(*value++);
				data3 <<= 4;
				data3 |= ToNibble(*value++);
			}
		}
		value++; // skip '-'
		{
			auto& data4 = uuid.GetData4();
			for (auto j = 0; j < sizeof(data4); j++)
			{
				data4[j] <<= 4;
				data4[j] |= ToNibble(*value++);
				data4[j] <<= 4;
				data4[j] |= ToNibble(*value++);
			}
		}
		value++; // skip '-'
		{
			auto& data5 = uuid.GetData5();
			for (auto j = 0; j < sizeof(data5); j++)
			{
				data5[j] <<= 4;
				data5[j] |= ToNibble(*value++);
				data5[j] <<= 4;
				data5[j] |= ToNibble(*value++);
			}
		}
		return uuid;
	}

	Uuid ToUuid(const char* value)
	{
		return ToUuidT(value);
	}

	Uuid ToUuid(const wchar_t* value)
	{
		return ToUuidT(value);
	}
}
