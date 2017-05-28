#include <Neat\Types.h>
#include <Neat\Convert.h>

#include <CppUnitTest.h>

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
			Assert::AreEqual("-128", Convert::ToUtf8((int8_t)(128)));
			Assert::AreEqual("-32768", Convert::ToUtf8((int16_t)(32768)));
			Assert::AreEqual("-2147483648", Convert::ToUtf8((int32_t)(2147483648)));
			Assert::AreEqual("-9223372036854775808", Convert::ToUtf8((int64_t)(9223372036854775808ULL)));

			Assert::AreEqual("128", Convert::ToUtf8((uint8_t)(128)));
			Assert::AreEqual("32768", Convert::ToUtf8((uint16_t)(32768)));
			Assert::AreEqual("2147483648", Convert::ToUtf8((uint32_t)(2147483648)));
			Assert::AreEqual("9223372036854775808", Convert::ToUtf8((uint64_t)(9223372036854775808ULL)));

			Assert::AreEqual(L"-128", Convert::ToUtf16((int8_t)(128)));
			Assert::AreEqual(L"-32768", Convert::ToUtf16((int16_t)(32768)));
			Assert::AreEqual(L"-2147483648", Convert::ToUtf16((int32_t)(2147483648)));
			Assert::AreEqual(L"-9223372036854775808", Convert::ToUtf16((int64_t)(9223372036854775808ULL)));

			Assert::AreEqual(L"128", Convert::ToUtf16((uint8_t)(128)));
			Assert::AreEqual(L"32768", Convert::ToUtf16((uint16_t)(32768)));
			Assert::AreEqual(L"2147483648", Convert::ToUtf16((uint32_t)(2147483648)));
			Assert::AreEqual(L"9223372036854775808", Convert::ToUtf16((uint64_t)(9223372036854775808ULL)));
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
				result = Convert::ToUtf8(source);

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
				result = Convert::ToUtf16(source);

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

				source = "0FAE01";
				result = Convert::ToBuffer(source);

				auto buffer = result.GetBuffer();
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

				source = L"0FAE01";
				result = Convert::ToBuffer(source);

				auto buffer = result.GetBuffer();
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
	};
}