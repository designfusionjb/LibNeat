#include <Neat\Types.h>
#include <Neat\Utf.h>
#include <Neat\StackAllocator.h>

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{		
	TEST_CLASS(UtfTest)
	{
	public:
		TEST_METHOD(String_Unintialized)
		{
			Utf8 string;
			
			Assert::IsTrue(string.IsEmpty());
			Assert::IsNull(string.GetBuffer());

			Assert::AreEqual(0_sz, string.GetSize());
			Assert::AreEqual(0_sz, string.GetLength());

			Utf8 copy(string);

			Assert::IsTrue(string.IsEmpty());
			Assert::IsNull(string.GetBuffer());

			Assert::AreEqual(0_sz, string.GetSize());
			Assert::AreEqual(0_sz, string.GetLength());

			Assert::IsTrue(copy.IsEmpty());
			Assert::IsNull(copy.GetBuffer());

			Assert::AreEqual(0_sz, copy.GetSize());
			Assert::AreEqual(0_sz, copy.GetLength());
		}

		TEST_METHOD(String_Empty)
		{
			Utf16 string(L"");
			
			Assert::IsTrue(string.IsEmpty());
			Assert::IsNotNull(string.GetBuffer());

			Assert::AreEqual(2_sz, string.GetSize());
			Assert::AreEqual(0_sz, string.GetLength());
		}

		TEST_METHOD(String_Basic)
		{
			Utf8 string("Some");
			
			Assert::IsFalse(string.IsEmpty());
			Assert::IsNotNull(string.GetBuffer());

			Assert::AreEqual(4_sz, string.GetLength());
			Assert::AreEqual(5_sz, string.GetSize());

			char* ptr = string;
			Assert::AreEqual(ptr, string);
			Assert::AreEqual("Some", string);
		}

		TEST_METHOD(String_Const)
		{
			const Utf16 string(L"Some");
			
			Assert::IsFalse(string.IsEmpty());
			Assert::IsNotNull(string.GetBuffer());

			Assert::AreEqual(4_sz, string.GetLength());
			Assert::AreEqual(10_sz, string.GetSize());

			const wchar_t* ptr = string;
			Assert::AreEqual(ptr, string);
			Assert::AreEqual(L"Some", string);
		}

		TEST_METHOD(String_GetLength)
		{
			Assert::AreEqual(0_sz, Utf8::GetLength(nullptr));
			Assert::AreEqual(0_sz, Utf16::GetLength(nullptr));

			Assert::AreEqual(0_sz, Utf8::GetLength(""));
			Assert::AreEqual(0_sz, Utf16::GetLength(L""));

			Assert::AreEqual(15_sz, Utf8::GetLength(u8"text текст"));
			Assert::AreEqual(10_sz, Utf16::GetLength(L"text текст"));
		}
	
#ifdef _DEBUG
		TEST_METHOD(String_FormatValidation)
		{
			// Should not throw
			Utf8::Format("");
			Utf8::Format("%i", 1);

			// Should throw "Bad format"
			Assert::ExpectException<std::exception>([]()
			{
				Utf8::Format("%i");
			});

			// Should throw "Invalid format char"
			Assert::ExpectException<std::exception>([]()
			{
				Utf8::Format("%y", 0);
			});

			// Should throw "Too few format specifiers"
			Assert::ExpectException<std::exception>([]()
			{
				Utf8::Format("%i", 0, 1);
			});

			// Should throw "Integer value expected"
			Assert::ExpectException<std::exception>([]()
			{
				Utf8::Format("%i", 0.1);
			});

			Utf8::Format("%f", 1.0);

			// Should throw "Floating point value expected"
			Assert::ExpectException<std::exception>([]()
			{
				Utf8::Format("%f", 1);
			});

			Utf8::Format("%p", this);

			// Should throw "Pointer expected"
			Assert::ExpectException<std::exception>([]()
			{
				Utf8::Format("%p", 1);
			});

			char array8[] = "char";
			char* pointer8 = array8;

			wchar_t array16[] = L"wchar_t";
			wchar_t* pointer16 = array16;

			Utf8::Format("%s", array8);
			Utf8::Format("%s", pointer8);
			Utf8::Format("%s", "char");

			Utf8::Format("%S", array16);
			Utf8::Format("%S", pointer16);
			Utf8::Format("%S", L"wchar_t");

			Utf16::Format(L"%s", array16);
			Utf16::Format(L"%s", pointer16);
			Utf16::Format(L"%s", L"wchar_t");

			Utf16::Format(L"%S", array8);
			Utf16::Format(L"%S", pointer8);
			Utf16::Format(L"%S", "char");

			int8_t arrayBad[1];
			auto pointerBad = arrayBad;

			// Should throw "Utf8 string expected"
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf8::Format("%s", array16);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf8::Format("%s", pointer16);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf8::Format("%s", L"wchar_t");
			});
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf8::Format("%s", arrayBad);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf8::Format("%s", pointerBad);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf8::Format("%s", 1);
			});
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf16::Format(L"%S", array16);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf16::Format(L"%S", pointer16);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf16::Format(L"%S", L"wchar_t");
			});
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf16::Format(L"%S", arrayBad);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf16::Format(L"%S", pointerBad);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf16::Format(L"%S", 1);
			});

			// Should throw "Utf16 string expected"
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf8::Format("%S", array8);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf8::Format("%S", pointer8);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf8::Format("%S", "char");
			});
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf8::Format("%S", arrayBad);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf8::Format("%S", pointerBad);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf8::Format("%S", 1);
			});
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf16::Format(L"%s", array8);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf16::Format(L"%s", pointer8);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf16::Format(L"%s", "char");
			});
			Assert::ExpectException<std::runtime_error>([&]()
			{
				Utf16::Format(L"%s", arrayBad);
			});
			Assert::ExpectException<std::runtime_error>([=]()
			{
				Utf16::Format(L"%s", pointerBad);
			});
			Assert::ExpectException<std::runtime_error>([]()
			{
				Utf16::Format(L"%s", 1);
			});
		}
#endif

#pragma warning(push)
#pragma warning(disable:4146)
		TEST_METHOD(String_FormatInteger)
		{
			Assert::AreEqual("-2147483648, -1, 0, 1, 2147483647", Utf8::Format("%i, %i, %i, %i, %i", -2147483648, -1, 0, 1, 2147483647));
			Assert::AreEqual(L"-2147483648, -1, 0, 1, 2147483647", Utf16::Format(L"%i, %i, %i, %i, %i", -2147483648, -1, 0, 1, 2147483647));

			Assert::AreEqual("-2147483648, -1, 0, 1, 2147483647", Utf8::Format("%li, %li, %li, %li, %li", -2147483648l, -1l, 0l, 1l, 2147483647l));
			Assert::AreEqual(L"-2147483648, -1, 0, 1, 2147483647", Utf16::Format(L"%li, %li, %li, %li, %li", -2147483648l, -1l, 0l, 1l, 2147483647l));

			Assert::AreEqual("-9223372036854775808, -1, 0, 1, 9223372036854775807", Utf8::Format("%lli, %lli, %lli, %lli, %lli", -9'223'372'036'854'775'808ll, -1ll, 0ll, 1ll, 9'223'372'036'854'775'807ll));
			Assert::AreEqual(L"-9223372036854775808, -1, 0, 1, 9223372036854775807", Utf16::Format(L"%lli, %lli, %lli, %lli, %lli", -9'223'372'036'854'775'808ll, -1ll, 0ll, 1ll, 9'223'372'036'854'775'807ll));

			Assert::AreEqual("-2147483648, -1, 0, 1, 2147483647", Utf8::Format("%d, %d, %d, %d, %d", -2147483648, -1, 0, 1, 2147483647));
			Assert::AreEqual(L"-2147483648, -1, 0, 1, 2147483647", Utf16::Format(L"%d, %d, %d, %d, %d", -2147483648, -1, 0, 1, 2147483647));

			Assert::AreEqual("-2147483648, -1, 0, 1, 2147483647", Utf8::Format("%ld, %ld, %ld, %ld, %ld", -2147483648l, -1l, 0l, 1l, 2147483647l));
			Assert::AreEqual(L"-2147483648, -1, 0, 1, 2147483647", Utf16::Format(L"%ld, %ld, %ld, %ld, %ld", -2147483648l, -1l, 0l, 1l, 2147483647l));

			Assert::AreEqual("-9223372036854775808, -1, 0, 1, 9223372036854775807", Utf8::Format("%lld, %lld, %lld, %lld, %lld", -9'223'372'036'854'775'808ll, -1ll, 0ll, 1ll, 9'223'372'036'854'775'807ll));
			Assert::AreEqual(L"-9223372036854775808, -1, 0, 1, 9223372036854775807", Utf16::Format(L"%lld, %lld, %lld, %lld, %lld", -9'223'372'036'854'775'808ll, -1ll, 0ll, 1ll, 9'223'372'036'854'775'807ll));

			Assert::AreEqual("4294967295, 0, 4294967295", Utf8::Format("%u, %u, %u", -1u, 0u, 4294967295u));
			Assert::AreEqual(L"4294967295, 0, 4294967295", Utf16::Format(L"%u, %u, %u", -1u, 0u, 4294967295u));

			Assert::AreEqual("4294967295, 0, 4294967295", Utf8::Format("%lu, %lu, %lu", -1lu, 0lu, 4294967295lu));
			Assert::AreEqual(L"4294967295, 0, 4294967295", Utf16::Format(L"%lu, %lu, %lu", -1lu, 0lu, 4294967295lu));

			Assert::AreEqual("18446744073709551615, 0, 18446744073709551615", Utf8::Format("%llu, %llu, %llu", -1llu, 0llu, 18446744073709551615llu));
			Assert::AreEqual(L"18446744073709551615, 0, 18446744073709551615", Utf16::Format(L"%llu, %llu, %llu", -1llu, 0llu, 18446744073709551615llu));

			Assert::AreEqual("0x0000A000", Utf8::Format("0x%08X", 0xA000));
			Assert::AreEqual(L"0x0000A000", Utf16::Format(L"0x%08X", 0xA000));
		}
#pragma warning(pop)
		
		TEST_METHOD(String_FormatFloating)
		{
			Assert::AreEqual("3.140000 2.71828", Utf8::Format("%f %g", 3.14, 2.718281828));
			Assert::AreEqual(L"3.140000 2.71828", Utf16::Format(L"%f %g", 3.14, 2.718281828));
		}
		
		TEST_METHOD(String_FormatString)
		{
			Assert::AreEqual(" ", Utf8::Format("%s %S", "", L""));
			Assert::AreEqual(L" ", Utf16::Format(L"%S %s", "", L""));

			Assert::AreEqual("(null) (null)", Utf8::Format("%s %S", (const char*)nullptr, (const wchar_t*)nullptr));
			Assert::AreEqual(L"(null) (null)", Utf16::Format(L"%S %s", (const char*)nullptr, (const wchar_t*)nullptr));

			Assert::AreEqual("ascii wide", Utf8::Format("%s %S", "ascii", L"wide"));
			Assert::AreEqual(L"ascii wide", Utf16::Format(L"%S %s", "ascii", L"wide"));

			std::string ascii("ascii");
			std::wstring wide(L"wide");

			Assert::AreEqual("ascii wide", Utf8::Format("%s %S", ascii, wide));
			Assert::AreEqual(L"ascii wide", Utf16::Format(L"%S %s", ascii, wide));

			Utf8 a("текст");
			Utf8 b("\xD1\x82\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82");
			Utf16 w(L"текст");

			Assert::AreEqual("(текст)", Utf8::Format("(%s)", a));
			Assert::AreEqual("(\xD1\x82\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82)", Utf8::Format("(%s)", b));
			Assert::AreEqual(L"(текст)", Utf16::Format(L"(%s)", w));

			// TODO: Uncoment when fixed
			//Assert::AreEqual(u8"Some thing C:\\Users\\¬се пользователи", Utf8::Format("%s %s %S", "Some", "thing", L"C:\\Users\\¬се пользователи"));
		}

		TEST_METHOD(String_CopyAssign)
		{
			Utf16 empty;
			Utf16 string(L"Some");

			Assert::IsNull(empty.GetBuffer());
			Assert::IsNotNull(string.GetBuffer());
			Assert::IsTrue(0 == empty.GetLength());
			Assert::IsTrue(4 == string.GetLength());
			
			const auto address = string.GetBuffer();
			empty = string;

			Assert::IsNotNull(empty.GetBuffer());
			Assert::IsNotNull(string.GetBuffer());
			Assert::IsTrue(4 == empty.GetLength());
			Assert::IsTrue(4 == string.GetLength());
			Assert::IsFalse(address == empty.GetBuffer());
		}

		TEST_METHOD(String_MoveAssign)
		{
			Utf16 empty;
			Utf16 string(L"Some");

			Assert::IsNull(empty.GetBuffer());
			Assert::IsNotNull(string.GetBuffer());
			Assert::IsTrue(0 == empty.GetLength());
			Assert::IsTrue(4 == string.GetLength());

			const auto address = string.GetBuffer();
			empty = std::move(string);

			Assert::IsNotNull(empty.GetBuffer());
			Assert::IsNull(string.GetBuffer());
			Assert::IsTrue(4 == empty.GetLength());
			Assert::IsTrue(0 == string.GetLength());
			Assert::IsTrue(address == empty.GetBuffer());
		}

		TEST_METHOD(String_CopyConstruct)
		{
			Utf16 string(L"Some");
			Utf16 copy(string);

			Assert::IsNotNull(string.GetBuffer());
			Assert::IsNotNull(copy.GetBuffer());
			Assert::IsTrue(4 == string.GetLength());
			Assert::IsTrue(4 == copy.GetLength());
			Assert::IsFalse(string.GetBuffer() == copy.GetBuffer());
		}

		TEST_METHOD(String_MoveConstruct)
		{
			Utf16 string(L"Some");
			const auto address = string.GetBuffer();
			Utf16 copy(std::move(string));

			Assert::IsNull(string.GetBuffer());
			Assert::IsNotNull(copy.GetBuffer());
			Assert::IsTrue(0 == string.GetLength());
			Assert::IsTrue(4 == copy.GetLength());
			Assert::IsTrue(address == copy.GetBuffer());
		}

		TEST_METHOD(String_AllocateFree)
		{
			Utf16 string;

			Assert::IsTrue(string.IsEmpty());
			Assert::IsNull(string.GetBuffer());

			Assert::AreEqual(0_sz, string.GetLength());
			Assert::AreEqual(0_sz, string.GetSize());

			string.Reserve(7);

			Assert::IsTrue(string.IsEmpty());
			Assert::IsNotNull(string.GetBuffer());

			Assert::AreEqual(0_sz, string.GetLength());
			Assert::AreNotEqual(0_sz, string.GetSize());

			string.Clear();

			Assert::IsTrue(string.IsEmpty());
			Assert::IsNull(string.GetBuffer());

			Assert::AreEqual(0_sz, string.GetLength());
			Assert::AreEqual(0_sz, string.GetSize());
		}

		TEST_METHOD(String_Reserve)
		{
			Utf16 string(L"some");
			string.Reserve(9);

			const auto before = string.GetBuffer();
			Assert::AreEqual(L"some", string);

			string = L"something";

			const auto after = string.GetBuffer();
			Assert::AreEqual(L"something", string);
			Assert::IsTrue(before == after);
		}

		TEST_METHOD(String_Match)
		{
			{
				Utf8 string("SomeMask_1234.zzz");

				Assert::AreEqual(true, string.Match("SomeMask_1234.zzz"));
				Assert::AreEqual(true, string.Match("somemask_1234.zzz"));
				Assert::AreEqual(true, string.Match("SOMEMASK_1234.ZZZ"));
				Assert::AreEqual(true, string.Match("sOMEmASK_1234.ZzZ"));
				Assert::AreEqual(true, string.Match("SomeMask_????.zzz"));
				Assert::AreEqual(true, string.Match("SomeMask_1234.*"));
				Assert::AreEqual(true, string.Match("SomeMask_????.*"));
				Assert::AreEqual(true, string.Match("*.*"));
				Assert::AreEqual(true, string.Match("*"));

				Assert::AreEqual(false, string.Match("SomeMask_??.zzz"));
				Assert::AreEqual(false, string.Match("SomeMask_????."));
				Assert::AreEqual(false, string.Match("SomeMask_*."));
				Assert::AreEqual(false, string.Match("SomeMask_00*.zzz"));
				Assert::AreEqual(false, string.Match("SomeMask_*.z"));
				Assert::AreEqual(false, string.Match("Other"));
			}
			{
				Utf16 string(L"SomeMask_1234.zzz");

				Assert::AreEqual(true, string.Match(L"SomeMask_1234.zzz"));
				Assert::AreEqual(true, string.Match(L"somemask_1234.zzz"));
				Assert::AreEqual(true, string.Match(L"SOMEMASK_1234.ZZZ"));
				Assert::AreEqual(true, string.Match(L"sOMEmASK_1234.ZzZ"));
				Assert::AreEqual(true, string.Match(L"SomeMask_????.zzz"));
				Assert::AreEqual(true, string.Match(L"SomeMask_1234.*"));
				Assert::AreEqual(true, string.Match(L"SomeMask_????.*"));
				Assert::AreEqual(true, string.Match(L"*.*"));
				Assert::AreEqual(true, string.Match(L"*"));

				Assert::AreEqual(false, string.Match(L"SomeMask_??.zzz"));
				Assert::AreEqual(false, string.Match(L"SomeMask_????."));
				Assert::AreEqual(false, string.Match(L"SomeMask_*."));
				Assert::AreEqual(false, string.Match(L"SomeMask_00*.zzz"));
				Assert::AreEqual(false, string.Match(L"SomeMask_*.z"));
				Assert::AreEqual(false, string.Match(L"Other"));
			}
		}

		TEST_METHOD(String_Find)
		{
			{
				Utf8 utf8;
				Utf16 utf16;

				Assert::AreEqual(0_sz, utf8.Find(nullptr));
				Assert::AreEqual(0_sz, utf16.Find(nullptr));
			}
			{
				Utf8 utf8("");
				Utf16 utf16(L"");

				Assert::AreEqual(0_sz, utf8.Find(nullptr));
				Assert::AreEqual(0_sz, utf16.Find(nullptr));
			}
			{
				Utf8 utf8;
				Utf16 utf16;

				Assert::AreEqual(0_sz, utf8.Find(""));
				Assert::AreEqual(0_sz, utf16.Find(L""));
			}
			{
				Utf8 utf8;
				Utf16 utf16;

				Assert::AreEqual(Utf8::End, utf8.Find("123"));
				Assert::AreEqual(Utf16::End, utf16.Find(L"123"));
			}
			{
				Utf8 utf8("123");
				Utf16 utf16(L"123");

				Assert::AreEqual(Utf8::End, utf8.Find("123", 1));
				Assert::AreEqual(Utf16::End, utf16.Find(L"123", 1));
			}
			{
				Utf8 utf8("123");
				Utf16 utf16(L"123");

				Assert::AreEqual(Utf8::End, utf8.Find("123", 4));
				Assert::AreEqual(Utf16::End, utf16.Find(L"123", 4));
			}
			{
				Utf8 utf8("");
				Utf16 utf16(L"");

				Assert::AreEqual(0_sz, utf8.Find(""));
				Assert::AreEqual(0_sz, utf16.Find(L""));
			}
			{
				Utf8 utf8("123");
				Utf16 utf16(L"123");

				Assert::AreEqual(0_sz, utf8.Find(""));
				Assert::AreEqual(0_sz, utf16.Find(L""));
			}
			{
				Utf8 utf8("123");
				Utf16 utf16(L"123");

				Assert::AreEqual(0_sz, utf8.Find("123"));
				Assert::AreEqual(0_sz, utf16.Find(L"123"));
			}
			{
				Utf8 utf8("123123");
				Utf16 utf16(L"123123");

				Assert::AreEqual(3_sz, utf8.Find("123", 3));
				Assert::AreEqual(3_sz, utf16.Find(L"123", 3));
			}
		}

		TEST_METHOD(String_FindLast)
		{
			{
				Utf8 utf8;
				Utf16 utf16;

				Assert::AreEqual(0_sz, utf8.FindLast(nullptr));
				Assert::AreEqual(0_sz, utf16.FindLast(nullptr));
			}
			{
				Utf8 utf8("");
				Utf16 utf16(L"");

				Assert::AreEqual(0_sz, utf8.FindLast(nullptr));
				Assert::AreEqual(0_sz, utf16.FindLast(nullptr));
			}
			{
				Utf8 utf8;
				Utf16 utf16;

				Assert::AreEqual(0_sz, utf8.FindLast(""));
				Assert::AreEqual(0_sz, utf16.FindLast(L""));
			}
			{
				Utf8 utf8("");
				Utf16 utf16(L"");

				Assert::AreEqual(0_sz, utf8.FindLast(""));
				Assert::AreEqual(0_sz, utf16.FindLast(L""));
			}
			{
				Utf8 utf8;
				Utf16 utf16;

				Assert::AreEqual(Utf8::End, utf8.FindLast("/"));
				Assert::AreEqual(Utf16::End, utf16.FindLast(L"/"));
			}
			// Match at the beginning
			{
				Utf8 utf8("/some");
				Utf16 utf16(L"/some");

				Assert::AreEqual(0_sz, utf8.FindLast("/"));
				Assert::AreEqual(0_sz, utf16.FindLast(L"/"));
			}
			// Match in the middle
			{
				Utf8 utf8("/some/path");
				Utf16 utf16(L"/some/path");

				Assert::AreEqual(5_sz, utf8.FindLast("/"));
				Assert::AreEqual(5_sz, utf16.FindLast(L"/"));
			}
			// Match at the end
			{
				Utf8 utf8("/some/path/");
				Utf16 utf16(L"/some/path/");

				Assert::AreEqual(10_sz, utf8.FindLast("/"));
				Assert::AreEqual(10_sz, utf16.FindLast(L"/"));
			}
			// Longer match
			{
				Utf8 utf8("/path/path");
				Utf16 utf16(L"/path/path");

				Assert::AreEqual(5_sz, utf8.FindLast("/path"));
				Assert::AreEqual(5_sz, utf16.FindLast(L"/path"));
			}
			// Query longer than string
			{
				Utf8 utf8("/foo");
				Utf16 utf16(L"/foo");

				Assert::AreEqual(Utf8::End, utf8.FindLast("/path"));
				Assert::AreEqual(Utf16::End, utf16.FindLast(L"/path"));
			}
			// Real world examples
			{
				Utf8 utf8(R"(C:\Dir\SubDir\FileName.Ext)");
				Utf16 utf16(LR"(C:\Dir\SubDir\FileName.Ext)");

				Assert::AreEqual(13_sz, utf8.FindLast("\\"));
				Assert::AreEqual(13_sz, utf16.FindLast(L"\\"));

				Assert::AreEqual(22_sz, utf8.FindLast("."));
				Assert::AreEqual(22_sz, utf16.FindLast(L"."));
			}
		}

		TEST_METHOD(String_Replace)
		{
			{
				Utf8 string("Some text with marker!");
				const auto before = string.GetBuffer();
				string.Replace("marker", "value");
				const auto after = string.GetBuffer();
				Assert::AreEqual("Some text with value!", string);
				Assert::IsTrue(before == after);
			}
			{
				Utf16 string(L"Some text with marker!");
				const auto before = string.GetBuffer();
				string.Replace(L"marker", L"value");
				const auto after = string.GetBuffer();
				Assert::AreEqual(L"Some text with value!", string);
				Assert::IsTrue(before == after);
			}
			// One to one replcement
			{
				Utf8 string("???");
				const auto before = string.GetBuffer();
				string.Replace("?", "!");
				const auto after = string.GetBuffer();
				Assert::AreEqual("!!!", string);
				Assert::IsTrue(before == after);
			}
			{
				Utf16 string(L"???");
				const auto before = string.GetBuffer();
				string.Replace(L"?", L"!");
				const auto after = string.GetBuffer();
				Assert::AreEqual(L"!!!", string);
				Assert::IsTrue(before == after);
			}
			// One byte -> three byte code point
			{
				Utf8 string(u8"$abc$def$");
				const auto before = string.GetBuffer();
				string.Replace(u8"$", u8"И");
				const auto after = string.GetBuffer();
				Assert::AreEqual(u8"ИabcИdefИ", string);
				Assert::IsFalse(before == after);
			}
			// Three byte -> one byte code point
			{
				Utf8 string(u8"ИabcИdefИ");
				const auto before = string.GetBuffer();
				string.Replace(u8"И", u8"$");
				const auto after = string.GetBuffer();
				Assert::AreEqual(u8"$abc$def$", string);
				Assert::IsTrue(before == after);
			}
			// Buffer isn't large enough, reallocation is expected
			{
				Utf8 string(20);
				string = "* some * text *";
				const auto before = string.GetBuffer();
				string.Replace("*", "123");
				const auto after = string.GetBuffer();
				Assert::AreEqual("123 some 123 text 123", string);
				Assert::IsFalse(before == after);
			}
			{
				Utf16 string(20);
				string = L"* some * text *";
				const auto before = string.GetBuffer();
				string.Replace(L"*", L"123");
				const auto after = string.GetBuffer();
				Assert::AreEqual(L"123 some 123 text 123", string);
				Assert::IsFalse(before == after);
			}
			// Buffer is large enough, reallocation isn't expected
			{
				Utf8 string(21);
				string = "* some * text *";
				const auto before = string.GetBuffer();
				string.Replace("*", "123");
				const auto after = string.GetBuffer();
				Assert::AreEqual("123 some 123 text 123", string);
				Assert::IsTrue(before == after);
			}
			{
				Utf16 string(21);
				string = L"* some * text *";
				const auto before = string.GetBuffer();
				string.Replace(L"*", L"123");
				const auto after = string.GetBuffer();
				Assert::AreEqual(L"123 some 123 text 123", string);
				Assert::IsTrue(before == after);
			}
			// Infinite loop test
			{
				Utf8 string = R"(C:\Windows\regedit.exe)";
				string.Replace(R"(C:\Windows)", R"(C:\Windows\SysWOW64)");
				Assert::AreEqual(R"(C:\Windows\SysWOW64\regedit.exe)", string);
			}
			{
				Utf16 string = LR"(C:\Windows\regedit.exe)";
				string.Replace(LR"(C:\Windows)", LR"(C:\Windows\SysWOW64)");
				Assert::AreEqual(LR"(C:\Windows\SysWOW64\regedit.exe)", string);
			}
			//
			// [from, End) replacement
			// TODO: add corner cases!!!
			//
			{
				Utf8 string = "foo bar";
				string.Replace(4, "hum");
				Assert::AreEqual("foo hum", string);
			}
			{
				Utf16 string = L"foo bar";
				string.Replace(4, L"hum");
				Assert::AreEqual(L"foo hum", string);
			}
			//
			// [from, to) replacement
			// TODO: add corner cases!!!
			//
			{
				Utf8 string = "foo %var% bar";
				string.Replace(4, 9, "hum");
				Assert::AreEqual("foo hum bar", string);
			}
			{
				Utf16 string = L"foo %var% bar";
				string.Replace(4, 9, L"hum");
				Assert::AreEqual(L"foo hum bar", string);
			}
		}

		TEST_METHOD(String_Substring)
		{
			{
				Utf16 string;
				const auto sub = string.Substring(0);
				Assert::AreEqual(nullptr, sub);
			}
			{
				Utf16 string(L"Some string");
				Assert::ExpectException<std::exception>([&]()
				{
					const auto sub = string.Substring(42);
				});
			}
			{
				Utf16 string(L"Some string");
				const auto sub = string.Substring(5);
				Assert::AreEqual(L"string", sub);
			}
			{
				Utf16 string(L"Some string");
				const auto sub = string.Substring(5, 42);
				Assert::AreEqual(L"string", sub);
			}
		}

		TEST_METHOD(String_Split)
		{
			{
				Utf8 string("A|B||D");
				Utf8 by("|");
				Utf8 token;

				size_t from = 0;
				size_t end = -1;

				Assert::AreNotEqual(end, string.Split(by, token, from));
				Assert::AreEqual("A", token);

				Assert::AreNotEqual(end, string.Split(by, token, from));
				Assert::AreEqual("B", token);

				Assert::AreNotEqual(end, string.Split(by, token, from));
				Assert::IsTrue(token.IsEmpty());

				Assert::AreNotEqual(end, string.Split(by, token, from));
				Assert::AreEqual("D", token);

				token = "E";
				Assert::AreEqual(end, string.Split(by, token, from));
				Assert::AreEqual("E", token);
			}
			{
				Utf8 string("line 1\r\nsecond line\r\nl3\r\n");
				Utf8 by("\r\n");
				Utf8 token;

				size_t start = 0;
				size_t end = -1;

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("line 1", token);

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("second line", token);

				Assert::AreNotEqual(end, string.Split(by, token, start));
				Assert::AreEqual("l3", token);

				Assert::AreEqual(end, string.Split(by, token, start));
			}
			{
				const auto string = Utf8("A|B||D");
				const auto by = "|";
				const auto tokenList = string.Split(by);

				Assert::IsTrue(3 == tokenList.size());
				Assert::AreEqual("A", tokenList[0]);
				Assert::AreEqual("B", tokenList[1]);
				Assert::AreEqual("D", tokenList[2]);
			}
			{
				const auto string = Utf8("A|B||D");
				const auto by = ",";
				const auto tokenList = string.Split(by);

				Assert::IsTrue(1 == tokenList.size());
				Assert::AreEqual("A|B||D", tokenList[0]);
			}
			{
				Utf8 string("A|B||D");
				Utf8 by("");
				Utf8 token;
				size_t start = 0;

				Assert::AreEqual(0_sz, string.Split(by, token, start));
				Assert::AreEqual(0_sz, string.Split(by, token, start));
			}
			{
				const auto string = Utf8("A|B||D");
				const char* tokenList[] = {"A", "B", "D", nullptr};

				auto i = 0_sz;
				for (const auto& token : string.Split("|"))
				{
					Assert::AreEqual(tokenList[i++], token);
				}
			}
		}

		TEST_METHOD(String_Trim)
		{
			{
				Utf16 string;
				string.Trim(L"");
				Assert::AreEqual(nullptr, string);
			}
			{
				Utf16 string(42);
				string.Trim(L"");
				Assert::AreEqual(L"", string);
			}
			{
				Utf16 string(L"Foo bar");
				string.Trim(nullptr);
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L"Foo bar");
				string.Trim(L"");
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L" Foo bar ");
				string.Trim(L"");
				Assert::AreEqual(L" Foo bar ", string);
			}
			{
				Utf16 string(L" Foo bar ");
				string.Trim(L" ");
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L"       Foo bar   ");
				string.Trim(L"\t ");
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L"\tFoo bar\t");
				string.Trim(L"\t");
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L"\t \t Foo bar \t \t");
				string.Trim(L" \t");
				Assert::AreEqual(L"Foo bar", string);
			}
		}

		TEST_METHOD(String_TrimLeft)
		{
			{
				Utf16 string;
				string.TrimLeft(L"");
				Assert::AreEqual(nullptr, string);
			}
			{
				Utf16 string(42);
				string.TrimLeft(L"");
				Assert::AreEqual(L"", string);
			}
			{
				Utf16 string(L"Foo bar");
				string.TrimLeft(nullptr);
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L"Foo bar");
				string.TrimLeft(L"");
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L" Foo bar ");
				string.TrimLeft(L"");
				Assert::AreEqual(L" Foo bar ", string);
			}
			{
				Utf16 string(L" Foo bar ");
				string.TrimLeft(L" ");
				Assert::AreEqual(L"Foo bar ", string);
			}
			{
				Utf16 string(L"       Foo bar   ");
				string.TrimLeft(L"\t ");
				Assert::AreEqual(L"Foo bar   ", string);
			}
			{
				Utf16 string(L"\tFoo bar\t");
				string.TrimLeft(L"\t");
				Assert::AreEqual(L"Foo bar\t", string);
			}
			{
				Utf16 string(L"\t \t Foo bar \t \t");
				string.TrimLeft(L" \t");
				Assert::AreEqual(L"Foo bar \t \t", string);
			}
		}

		TEST_METHOD(String_TrimRight)
		{
			{
				Utf16 string;
				string.TrimRight(L"");
				Assert::AreEqual(nullptr, string);
			}
			{
				Utf16 string(42);
				string.TrimRight(L"");
				Assert::AreEqual(L"", string);
			}
			{
				Utf16 string(L"Foo bar");
				string.TrimRight(nullptr);
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L"Foo bar");
				string.TrimRight(L"");
				Assert::AreEqual(L"Foo bar", string);
			}
			{
				Utf16 string(L" Foo bar ");
				string.TrimRight(L"");
				Assert::AreEqual(L" Foo bar ", string);
			}
			{
				Utf16 string(L" Foo bar ");
				string.TrimRight(L" ");
				Assert::AreEqual(L" Foo bar", string);
			}
			{
				Utf16 string(L"       Foo bar   ");
				string.TrimRight(L"\t ");
				Assert::AreEqual(L"       Foo bar", string);
			}
			{
				Utf16 string(L"\tFoo bar\t");
				string.TrimRight(L"\t");
				Assert::AreEqual(L"\tFoo bar", string);
			}
			{
				Utf16 string(L"\t \t Foo bar \t \t");
				string.TrimRight(L" \t");
				Assert::AreEqual(L"\t \t Foo bar", string);
			}
		}

		TEST_METHOD(String_IsEqual)
		{
			{
				const wchar_t* ptr = L"value";
				Utf16 string(ptr);

				Assert::IsTrue(string.IsEqual(ptr));
				Assert::IsTrue(string == ptr);
				Assert::IsTrue(ptr == string);
			}
			{
				const wchar_t* ptr = L"value";
				Utf16 string(L"value");

				Assert::IsTrue(string.IsEqual(ptr));
				Assert::IsTrue(string == ptr);
				Assert::IsTrue(ptr == string);
			}
			{
				const wchar_t* ptr = L"value1";
				Utf16 string(L"value2");

				Assert::IsFalse(string.IsEqual(ptr));
				Assert::IsFalse(string == ptr);
				Assert::IsFalse(ptr == string);
			}
			{
				const wchar_t* ptr = nullptr;
				Utf16 string(L"value");

				Assert::IsFalse(string.IsEqual(ptr));
				Assert::IsFalse(string == ptr);
				Assert::IsFalse(ptr == string);
			}
			{
				const wchar_t* ptr = L"value";
				Utf16 string;

				Assert::IsFalse(string.IsEqual(ptr));
				Assert::IsFalse(string == ptr);
				Assert::IsFalse(ptr == string);
			}
			{
				const wchar_t* ptr = nullptr;
				Utf16 string;

				Assert::IsTrue(string.IsEqual(ptr));
				Assert::IsTrue(string == ptr);
				Assert::IsTrue(ptr == string);
			}
			{
				Utf16 string1;
				Utf16 string2;

				Assert::IsTrue(string1.IsEqual(string2));
				Assert::IsTrue(string1 == string2);
			}
			{
				Utf16 string1(L"value1");
				Utf16 string2(L"value2");

				Assert::IsFalse(string1.IsEqual(string2));
				Assert::IsFalse(string1 == string2);
			}
		}

		TEST_METHOD(String_BeginsWith)
		{
			{
				Assert::IsTrue(Utf8().BeginsWith(nullptr));
				Assert::IsTrue(Utf8().BeginsWith(""));
				Assert::IsTrue(Utf8("").BeginsWith(nullptr));
				Assert::IsTrue(Utf8("").BeginsWith(""));
				Assert::IsTrue(Utf8("Abc").BeginsWith("A"));
				Assert::IsFalse(Utf8("Abc").BeginsWith("b"));
			}
			{
				Assert::IsTrue(Utf16().BeginsWith(nullptr));
				Assert::IsTrue(Utf16().BeginsWith(L""));
				Assert::IsTrue(Utf16(L"").BeginsWith(nullptr));
				Assert::IsTrue(Utf16(L"").BeginsWith(L""));
				Assert::IsTrue(Utf16(L"Abc").BeginsWith(L"A"));
				Assert::IsFalse(Utf16(L"Abc").BeginsWith(L"b"));
			}
		}

		TEST_METHOD(String_EndsWith)
		{
			{
				Assert::IsTrue(Utf8().EndsWith(nullptr));
				Assert::IsTrue(Utf8().EndsWith(""));
				Assert::IsTrue(Utf8("").EndsWith(nullptr));
				Assert::IsTrue(Utf8("").EndsWith(""));
				Assert::IsTrue(Utf8("Some").EndsWith(""));
				Assert::IsTrue(Utf8("Some").EndsWith("Some"));
				Assert::IsTrue(Utf8("First, second, etc.").EndsWith("etc."));
				Assert::IsTrue(Utf8("Some\\Path\\").EndsWith("\\"));
				Assert::IsFalse(Utf8("Som").EndsWith("Some"));
				Assert::IsFalse(Utf8("First, second, etc.").EndsWith("etc.."));
				Assert::IsFalse(Utf8("Some\\Path").EndsWith("\\"));
			}
			{
				Assert::IsTrue(Utf16().EndsWith(nullptr));
				Assert::IsTrue(Utf16().EndsWith(L""));
				Assert::IsTrue(Utf16(L"").EndsWith(nullptr));
				Assert::IsTrue(Utf16(L"").EndsWith(L""));
				Assert::IsTrue(Utf16(L"Some").EndsWith(L""));
				Assert::IsTrue(Utf16(L"Some").EndsWith(L"Some"));
				Assert::IsTrue(Utf16(L"First, second, etc.").EndsWith(L"etc."));
				Assert::IsTrue(Utf16(L"Some\\Path\\").EndsWith(L"\\"));
				Assert::IsFalse(Utf16(L"Som").EndsWith(L"Some"));
				Assert::IsFalse(Utf16(L"First, second, etc.").EndsWith(L"etc.."));
				Assert::IsFalse(Utf16(L"Some\\Path").EndsWith(L"\\"));
			}
		}

		TEST_METHOD(String_Contains)
		{
			{
				Assert::IsTrue(Utf8().Contains(nullptr));
				Assert::IsTrue(Utf8().Contains(""));
				Assert::IsTrue(Utf8("").Contains(nullptr));
				Assert::IsTrue(Utf8("").Contains(""));
				Assert::IsTrue(Utf8("Some").Contains(""));
				Assert::IsTrue(Utf8("Some").Contains("Some"));
				Assert::IsTrue(Utf8("First, second, etc.").Contains("etc."));
				Assert::IsFalse(Utf8().Contains("Some"));
				Assert::IsFalse(Utf8("Som").Contains("Some"));
				Assert::IsFalse(Utf8("First, second, etc.").Contains("etc.."));
			}
			{
				Assert::IsTrue(Utf16().Contains(nullptr));
				Assert::IsTrue(Utf16().Contains(L""));
				Assert::IsTrue(Utf16(L"").Contains(nullptr));
				Assert::IsTrue(Utf16(L"").Contains(L""));
				Assert::IsTrue(Utf16(L"Some").Contains(L""));
				Assert::IsTrue(Utf16(L"Some").Contains(L"Some"));
				Assert::IsTrue(Utf16(L"First, second, etc.").Contains(L"etc."));
				Assert::IsFalse(Utf16().Contains(L"Some"));
				Assert::IsFalse(Utf16(L"Som").Contains(L"Some"));
				Assert::IsFalse(Utf16(L"First, second, etc.").Contains(L"etc.."));
			}
		}

		TEST_METHOD(String_ToLower)
		{
			{
				Utf16 string(L"");
				string.ToLower();
				Assert::AreEqual(L"", string);
			}
			{
				Utf16 string(L"abcdefghijklmnopqrstuvwxyz");
				string.ToLower();
				Assert::AreEqual(L"abcdefghijklmnopqrstuvwxyz", string);
			}
			{
				Utf16 string(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				string.ToLower();
				Assert::AreEqual(L"abcdefghijklmnopqrstuvwxyz", string);
			}
			{
				Utf16 string(L"Some0123");
				string.ToLower();
				Assert::AreEqual(L"some0123", string);
			}
			{
				Utf16 string(L"100 KB");
				string.ToLower();
				Assert::AreEqual(L"100 kb", string);
			}
			{
				Utf16 string(L"ћои ƒокументы");
				string.ToLower();
				Assert::AreEqual(L"мои документы", string);
			}
		}

		TEST_METHOD(String_ToUpper)
		{
			{
				Utf16 string(L"");
				string.ToUpper();
				Assert::AreEqual(L"", string);
			}
			{
				Utf16 string(L"abcdefghijklmnopqrstuvwxyz");
				string.ToUpper();
				Assert::AreEqual(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", string);
			}
			{
				Utf16 string(L"abcdefghijklmnopqrstuvwxyz");
				string.ToUpper();
				Assert::AreEqual(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", string);
			}
			{
				Utf16 string(L"some 123");
				string.ToUpper();
				Assert::AreEqual(L"SOME 123", string);
			}
			{
				Utf16 string(L"100kb");
				string.ToUpper();
				Assert::AreEqual(L"100KB", string);
			}
			{
				Utf16 string(L"ћои ƒокументы");
				string.ToUpper();
				Assert::AreEqual(L"ћќ» ƒќ ”ћ≈Ќ“џ", string);
			}
		}

		TEST_METHOD(String_Append)
		{
			{
				Utf8 string;
				Assert::AreEqual(nullptr, string);
				string.Append("C:");
				Assert::AreEqual("C:", string);
				string.Append('\\');
				Assert::AreEqual("C:\\", string);
				string.Append("Windows");
				Assert::AreEqual("C:\\Windows", string);
				string.Append('\\');
				Assert::AreEqual("C:\\Windows\\", string);
			}
			{
				Utf16 string;
				Assert::AreEqual(nullptr, string);
				string.Append(L"C:");
				Assert::AreEqual(L"C:", string);
				string.Append(L'\\');
				Assert::AreEqual(L"C:\\", string);
				string.Append(L"Windows");
				Assert::AreEqual(L"C:\\Windows", string);
				string.Append(L'\\');
				Assert::AreEqual(L"C:\\Windows\\", string);
			}
			{
				Utf8 string("Foo");
				Assert::AreEqual("Foo", string);
				string.Append("");
				Assert::AreEqual("Foo", string);
				string.Append("BarHum", 0);
				Assert::AreEqual("Foo", string);
				string.Append("BarHum", 3);
				Assert::AreEqual("FooBar", string);
			}
			{
				Utf16 string(L"Foo");
				Assert::AreEqual(L"Foo", string);
				string.Append(L"");
				Assert::AreEqual(L"Foo", string);
				string.Append(L"BarHum", 0);
				Assert::AreEqual(L"Foo", string);
				string.Append(L"BarHum", 3);
				Assert::AreEqual(L"FooBar", string);
			}
		}

		TEST_METHOD(String_Assign)
		{
			{
				Utf8 string;

				Assert::IsTrue(string.IsEmpty());
				Assert::IsNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() == 0);
				Assert::IsTrue(string.GetLength() == 0);

				string = "";

				Assert::IsTrue(string.IsEmpty());
				Assert::IsNotNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() > 0);
				Assert::IsTrue(string.GetLength() == 0);
			}
			{
				Utf16 string;

				Assert::IsTrue(string.IsEmpty());
				Assert::IsNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() == 0);
				Assert::IsTrue(string.GetLength() == 0);

				string = L"";

				Assert::IsTrue(string.IsEmpty());
				Assert::IsNotNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() > 0);
				Assert::IsTrue(string.GetLength() == 0);
			}
			{
				Utf8 string;

				Assert::IsTrue(string.IsEmpty());
				Assert::IsNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() == 0);
				Assert::IsTrue(string.GetLength() == 0);

				string = "Some";

				Assert::IsFalse(string.IsEmpty());
				Assert::IsNotNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() > 0);
				Assert::IsTrue(string.GetLength() == 4);
			}
			{
				Utf16 string;

				Assert::IsTrue(string.IsEmpty());
				Assert::IsNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() == 0);
				Assert::IsTrue(string.GetLength() == 0);

				string = L"Some";

				Assert::IsFalse(string.IsEmpty());
				Assert::IsNotNull(string.GetBuffer());

				Assert::IsTrue(string.GetSize() > 0);
				Assert::IsTrue(string.GetLength() == 4);
			}
		}

		TEST_METHOD(String_LessThan)
		{
			{
				Assert::IsTrue(Utf8() < Utf8(""));
				Assert::IsTrue(Utf8("abc") < Utf8("bcd"));

				Assert::IsFalse(Utf8() < Utf8());
				Assert::IsFalse(Utf8("") < Utf8());
				Assert::IsFalse(Utf8("bcd") < Utf8("abc"));
			}
			{
				Assert::IsTrue(Utf16() < Utf16(L""));
				Assert::IsTrue(Utf16(L"abc") < Utf16(L"bcd"));

				Assert::IsFalse(Utf16() < Utf16());
				Assert::IsFalse(Utf16(L"") < Utf16());
				Assert::IsFalse(Utf16(L"bcd") < Utf16(L"abc"));
			}
		}

		TEST_METHOD(String_CustomAllocator)
		{
			{
				StackAllocator<20> alloc;
				Utf8 string(&alloc);
				Assert::IsTrue(string.GetAllocator() == &alloc);

				string = "Hello ";
				string += "World!";
				Assert::AreEqual("Hello World!", string);
				Assert::IsTrue(string.GetAllocator() == &alloc);

				const auto capacity = alloc.GetCapacity();
				Assert::AreEqual(0_sz, capacity);
			}
			{
				StackAllocator<40> alloc;
				Utf16 string(&alloc);
				Assert::IsTrue(string.GetAllocator() == &alloc);

				string = L"Hello ";
				string += L"World!";
				Assert::AreEqual(L"Hello World!", string);
				Assert::IsTrue(string.GetAllocator() == &alloc);

				const auto capacity = alloc.GetCapacity();
				Assert::AreEqual(0_sz, capacity);
			}
		}
	};
}