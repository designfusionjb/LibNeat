#include <Neat\Types.h>
#include <Neat\Convert.h>

#include <chrono>

#include <CppUnitTest.h>
#include <objbase.h>
#pragma comment(lib, "Ole32.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{		
	TEST_CLASS(ConvertTest)
	{
	public:
		TEST_METHOD(Convert_StringToString)
		{
			// UTF8
			Assert::AreEqual(u8"text текст", Convert::ToUtf8(u8"text текст"));
			Assert::AreEqual(u8"text текст", Convert::ToUtf8(L"text текст"));
			Assert::AreEqual(u8"text текст", Convert::ToUtf8(Utf8(u8"text текст")));
			Assert::AreEqual(u8"text текст", Convert::ToUtf8(Utf16(L"text текст")));
			Assert::AreEqual(u8"text текст", Convert::ToUtf8(std::string(u8"text текст")));
			Assert::AreEqual(u8"text текст", Convert::ToUtf8(std::wstring(L"text текст")));

			// UTF16
			Assert::AreEqual(L"text текст", Convert::ToUtf16(u8"text текст"));
			Assert::AreEqual(L"text текст", Convert::ToUtf16(L"text текст"));
			Assert::AreEqual(L"text текст", Convert::ToUtf16(Utf8(u8"text текст")));
			Assert::AreEqual(L"text текст", Convert::ToUtf16(Utf16(L"text текст")));
			Assert::AreEqual(L"text текст", Convert::ToUtf16(std::string(u8"text текст")));
			Assert::AreEqual(L"text текст", Convert::ToUtf16(std::wstring(L"text текст")));
		}

		TEST_METHOD(Convert_IntegerToString)
		{
			// UTF8

			Assert::AreEqual("-128", Convert::ToUtf8(INT8_MIN));
			Assert::AreEqual("-32768", Convert::ToUtf8(INT16_MIN));
			Assert::AreEqual("-2147483648", Convert::ToUtf8(INT32_MIN));
			Assert::AreEqual("-9223372036854775808", Convert::ToUtf8(INT64_MIN));

			Assert::AreEqual("127", Convert::ToUtf8(INT8_MAX));
			Assert::AreEqual("32767", Convert::ToUtf8(INT16_MAX));
			Assert::AreEqual("2147483647", Convert::ToUtf8(INT32_MAX));
			Assert::AreEqual("9223372036854775807", Convert::ToUtf8(INT64_MAX));

			Assert::AreEqual("255", Convert::ToUtf8(UINT8_MAX));
			Assert::AreEqual("65535", Convert::ToUtf8(UINT16_MAX));
			Assert::AreEqual("4294967295", Convert::ToUtf8(UINT32_MAX));
			Assert::AreEqual("18446744073709551615", Convert::ToUtf8(UINT64_MAX));

			Assert::AreEqual("ff", Convert::ToUtf8(UINT8_MAX, Convert::Base::Hex));
			Assert::AreEqual("ffff", Convert::ToUtf8(UINT16_MAX, Convert::Base::Hex));
			Assert::AreEqual("ffffffff", Convert::ToUtf8(UINT32_MAX, Convert::Base::Hex));
			Assert::AreEqual("ffffffffffffffff", Convert::ToUtf8(UINT64_MAX, Convert::Base::Hex));

			Assert::AreEqual("01", Convert::ToUtf8(0x01ui8, Convert::Base::Hex));
			Assert::AreEqual("0123", Convert::ToUtf8(0x0123ui16, Convert::Base::Hex));
			Assert::AreEqual("01234567", Convert::ToUtf8(0x01234567ui32, Convert::Base::Hex));
			Assert::AreEqual("0123456789abcdef", Convert::ToUtf8(0x0123456789abcdefui64, Convert::Base::Hex));

			Assert::AreEqual("FF", Convert::ToUtf8(UINT8_MAX, Convert::Base::HexUp));
			Assert::AreEqual("FFFF", Convert::ToUtf8(UINT16_MAX, Convert::Base::HexUp));
			Assert::AreEqual("FFFFFFFF", Convert::ToUtf8(UINT32_MAX, Convert::Base::HexUp));
			Assert::AreEqual("FFFFFFFFFFFFFFFF", Convert::ToUtf8(UINT64_MAX, Convert::Base::HexUp));

			Assert::AreEqual("01", Convert::ToUtf8(0x01ui8, Convert::Base::HexUp));
			Assert::AreEqual("0123", Convert::ToUtf8(0x0123ui16, Convert::Base::HexUp));
			Assert::AreEqual("01234567", Convert::ToUtf8(0x01234567ui32, Convert::Base::HexUp));
			Assert::AreEqual("0123456789ABCDEF", Convert::ToUtf8(0x0123456789abcdefui64, Convert::Base::HexUp));

			// UTF16

			Assert::AreEqual(L"-128", Convert::ToUtf16(INT8_MIN));
			Assert::AreEqual(L"-32768", Convert::ToUtf16(INT16_MIN));
			Assert::AreEqual(L"-2147483648", Convert::ToUtf16(INT32_MIN));
			Assert::AreEqual(L"-9223372036854775808", Convert::ToUtf16(INT64_MIN));

			Assert::AreEqual(L"127", Convert::ToUtf16(INT8_MAX));
			Assert::AreEqual(L"32767", Convert::ToUtf16(INT16_MAX));
			Assert::AreEqual(L"2147483647", Convert::ToUtf16(INT32_MAX));
			Assert::AreEqual(L"9223372036854775807", Convert::ToUtf16(INT64_MAX));

			Assert::AreEqual(L"255", Convert::ToUtf16(UINT8_MAX));
			Assert::AreEqual(L"65535", Convert::ToUtf16(UINT16_MAX));
			Assert::AreEqual(L"4294967295", Convert::ToUtf16(UINT32_MAX));
			Assert::AreEqual(L"18446744073709551615", Convert::ToUtf16(UINT64_MAX));

			Assert::AreEqual(L"ff", Convert::ToUtf16(UINT8_MAX, Convert::Base::Hex));
			Assert::AreEqual(L"ffff", Convert::ToUtf16(UINT16_MAX, Convert::Base::Hex));
			Assert::AreEqual(L"ffffffff", Convert::ToUtf16(UINT32_MAX, Convert::Base::Hex));
			Assert::AreEqual(L"ffffffffffffffff", Convert::ToUtf16(UINT64_MAX, Convert::Base::Hex));

			Assert::AreEqual(L"01", Convert::ToUtf16(0x01ui8, Convert::Base::Hex));
			Assert::AreEqual(L"0123", Convert::ToUtf16(0x0123ui16, Convert::Base::Hex));
			Assert::AreEqual(L"01234567", Convert::ToUtf16(0x01234567ui32, Convert::Base::Hex));
			Assert::AreEqual(L"0123456789abcdef", Convert::ToUtf16(0x0123456789abcdefui64, Convert::Base::Hex));

			Assert::AreEqual(L"FF", Convert::ToUtf16(UINT8_MAX, Convert::Base::HexUp));
			Assert::AreEqual(L"FFFF", Convert::ToUtf16(UINT16_MAX, Convert::Base::HexUp));
			Assert::AreEqual(L"FFFFFFFF", Convert::ToUtf16(UINT32_MAX, Convert::Base::HexUp));
			Assert::AreEqual(L"FFFFFFFFFFFFFFFF", Convert::ToUtf16(UINT64_MAX, Convert::Base::HexUp));

			Assert::AreEqual(L"01", Convert::ToUtf16(0x01ui8, Convert::Base::HexUp));
			Assert::AreEqual(L"0123", Convert::ToUtf16(0x0123ui16, Convert::Base::HexUp));
			Assert::AreEqual(L"01234567", Convert::ToUtf16(0x01234567ui32, Convert::Base::HexUp));
			Assert::AreEqual(L"0123456789ABCDEF", Convert::ToUtf16(0x0123456789abcdefui64, Convert::Base::HexUp));

			// Perfomance comparison

			using namespace std::chrono;
			const auto count = 100u;
			{
				char utf8[21] = { 0 };
				wchar_t utf16[21] = { 0 };

				const auto start = steady_clock::now();
				for (auto i = 0u; i < count; i++)
				{
					::_itoa_s(i, utf8, 10);
					::_itow_s(i, utf16, 10);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u _itoa_s/_itow_s calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			{
				const auto start = steady_clock::now();
				for (auto i = 0u; i < count; i++)
				{
					volatile auto utf8 = Convert::ToUtf8(i);
					volatile auto utf16 = Convert::ToUtf16(i);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u convert to Dec calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			{
				const auto start = steady_clock::now();
				for (auto i = 0u; i < count; i++)
				{
					volatile auto utf8 = Convert::ToUtf8(i, Convert::Base::HexUp);
					volatile auto utf16 = Convert::ToUtf16(i, Convert::Base::HexUp);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u convert to Hex calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			Logger::WriteMessage(L"#");
		}

		TEST_METHOD(Convert_BufferToString)
		{
			{
				Buffer source;
				auto result = Convert::ToUtf8(source);
				Assert::AreEqual(true, result.IsEmpty());

				byte_t code[] = { 0x0F, 0xAE, 0x01 };
				size_t codeSize = sizeof(code);

				source = Buffer(code, codeSize);
				result = Convert::ToUtf8(source, Convert::Base::Hex);

				Assert::AreEqual(6_sz, result.GetLength());
				Assert::AreEqual("0fae01", result);
			}
			{
				Buffer source;
				auto result = Convert::ToUtf8(source);
				Assert::AreEqual(true, result.IsEmpty());

				byte_t code[] = { 0x0F, 0xAE, 0x01 };
				size_t codeSize = sizeof(code);

				source = Buffer(code, codeSize);
				result = Convert::ToUtf8(source, Convert::Base::HexUp);

				Assert::AreEqual(6_sz, result.GetLength());
				Assert::AreEqual("0FAE01", result);
			}
			{
				Buffer source;
				auto result = Convert::ToUtf16(source);
				Assert::AreEqual(true, result.IsEmpty());

				byte_t code[] = { 0x0F, 0xAE, 0x01 };
				size_t codeSize = sizeof(code);

				source = Buffer(code, codeSize);
				result = Convert::ToUtf16(source, Convert::Base::Hex);

				Assert::AreEqual(6_sz, result.GetLength());
				Assert::AreEqual(L"0fae01", result);
			}
			{
				Buffer source;
				auto result = Convert::ToUtf16(source);
				Assert::AreEqual(true, result.IsEmpty());

				byte_t code[] = { 0x0F, 0xAE, 0x01 };
				size_t codeSize = sizeof(code);

				source = Buffer(code, codeSize);
				result = Convert::ToUtf16(source, Convert::Base::HexUp);

				Assert::AreEqual(6_sz, result.GetLength());
				Assert::AreEqual(L"0FAE01", result);
			}
		}

		TEST_METHOD(Convert_StringToBuffer)
		{
			{
				Utf8 source;
				auto result = Convert::ToBuffer(source);
				Assert::AreEqual(true, result.IsEmpty());

				source = "0fae01";
				result = Convert::ToBuffer(source);

				auto buffer = result.GetBuffer();
				Assert::AreEqual(3_sz, result.GetSize());
				Assert::AreEqual(0x0F, (int)buffer[0]);
				Assert::AreEqual(0xAE, (int)buffer[1]);
				Assert::AreEqual(0x01, (int)buffer[2]);

				source = "0FAE01";
				result = Convert::ToBuffer(source);

				buffer = result.GetBuffer();
				Assert::AreEqual(3_sz, result.GetSize());
				Assert::AreEqual(0x0F, (int)buffer[0]);
				Assert::AreEqual(0xAE, (int)buffer[1]);
				Assert::AreEqual(0x01, (int)buffer[2]);

				Assert::ExpectException<std::runtime_error>([]()
				{
					Convert::ToBuffer("Foo");
				});
			}
			{
				Utf16 source;
				auto result = Convert::ToBuffer(source);
				Assert::AreEqual(true, result.IsEmpty());

				source = L"0fae01";
				result = Convert::ToBuffer(source);

				auto buffer = result.GetBuffer();
				Assert::AreEqual(3_sz, result.GetSize());
				Assert::AreEqual(0x0F, (int)buffer[0]);
				Assert::AreEqual(0xAE, (int)buffer[1]);
				Assert::AreEqual(0x01, (int)buffer[2]);

				source = L"0FAE01";
				result = Convert::ToBuffer(source);

				buffer = result.GetBuffer();
				Assert::AreEqual(3_sz, result.GetSize());
				Assert::AreEqual(0x0F, (int)buffer[0]);
				Assert::AreEqual(0xAE, (int)buffer[1]);
				Assert::AreEqual(0x01, (int)buffer[2]);

				Assert::ExpectException<std::runtime_error>([]()
				{
					Convert::ToBuffer(L"Foo");
				});
			}
		}

		TEST_METHOD(Convert_UuidToString)
		{
			// Sanity check

			Uuid uuid = { 0xa1, 0xeb, 0x53, 0xe9, 0xc8, 0xef, 0xf7, 0x45, 0x86, 0xdd, 0x01, 0xef, 0x20, 0x1f, 0xe2, 0x57 };

			Assert::AreEqual("e953eba1-efc8-45f7-86dd-01ef201fe257", Convert::ToUtf8(uuid, Convert::Base::Hex));
			Assert::AreEqual("E953EBA1-EFC8-45F7-86DD-01EF201FE257", Convert::ToUtf8(uuid, Convert::Base::HexUp));

			Assert::AreEqual(L"e953eba1-efc8-45f7-86dd-01ef201fe257", Convert::ToUtf16(uuid, Convert::Base::Hex));
			Assert::AreEqual(L"E953EBA1-EFC8-45F7-86DD-01EF201FE257", Convert::ToUtf16(uuid, Convert::Base::HexUp));

			// Perfomance comparison

			using namespace std::chrono;
			const auto count = 100;
			{
				const auto size = 39_sz;
				wchar_t utf16[size] = { 0 };

				GUID guid = { 0xE953EBA1, 0xEFC8, 0x45F7, { 0x86, 0xDD, 0x01, 0xEF, 0x20, 0x1F, 0xE2, 0x57 } };

				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					::StringFromGUID2(guid, utf16, size);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u ::StringFromGUID2() calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			{
				Uuid uuid = { 0xA1, 0xEB, 0x53, 0xE9, 0xC8, 0xEF, 0xF7, 0x45, 0x86, 0xDD, 0x01, 0xEF, 0x20, 0x1F, 0xE2, 0x57 };

				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					volatile auto utf16 = Convert::ToUtf16(uuid);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u Convert::ToUtf16() calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			Logger::WriteMessage(L"#");
		}

		TEST_METHOD(Convert_StringToUuid)
		{
			// Sanity check
			{
				Assert::ExpectException<std::exception>([]()
				{
					Convert::ToUuid(static_cast<const char*>(nullptr));
				});

				Assert::ExpectException<std::exception>([]()
				{
					Convert::ToUuid(static_cast<const wchar_t*>(nullptr));
				});

				Assert::ExpectException<std::exception>([]()
				{
					Convert::ToUuid("");
				});

				Assert::ExpectException<std::exception>([]()
				{
					Convert::ToUuid(L"");
				});

				Uuid uuid = { 0xA1, 0xEB, 0x53, 0xE9, 0xC8, 0xEF, 0xF7, 0x45, 0x86, 0xDD, 0x01, 0xEF, 0x20, 0x1F, 0xE2, 0x57 };
				Assert::IsTrue(uuid == Convert::ToUuid("e953eba1-efc8-45f7-86dd-01ef201fe257"));
				Assert::IsTrue(uuid == Convert::ToUuid("E953EBA1-EFC8-45F7-86DD-01EF201FE257"));
				Assert::IsTrue(uuid == Convert::ToUuid(L"e953eba1-efc8-45f7-86dd-01ef201fe257"));
				Assert::IsTrue(uuid == Convert::ToUuid(L"E953EBA1-EFC8-45F7-86DD-01EF201FE257"));
			}

			// Perfomance comparison

			using namespace std::chrono;
			const auto count = 100;
			{
				GUID guid;
				const auto string = L"{E953EBA1-EFC8-45F7-86DD-01EF201FE257}";

				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					::IIDFromString(string, &guid);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u ::IIDFromString() calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			{
				const auto string = L"E953EBA1-EFC8-45F7-86DD-01EF201FE257";
				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					volatile auto uuid = Convert::ToUuid(string);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u Convert::ToUuid() calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			Logger::WriteMessage(L"#");
		}
	};
}