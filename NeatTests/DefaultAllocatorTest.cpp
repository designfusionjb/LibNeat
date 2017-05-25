#include "IntToString.h"
#include <CppUnitTest.h>

#include <Neat\Types.h>
#include <Neat\DefaultAllocator.h>

#include <list>
#include <map>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{
	TEST_CLASS(DefaultAllocatorTest)
	{
	public:
		TEST_METHOD(DefaultAllocator_Vector)
		{
			{
				DefaultAllocator alloc;
				std::vector<int64_t, Allocator<int64_t>> vec(&alloc);
				vec.push_back(0);
				vec.push_back(INT64_MIN);
				vec.push_back(INT64_MAX);

				Assert::AreEqual(3_sz, vec.size());
				Assert::AreEqual(0i64, vec[0]);
				Assert::AreEqual(INT64_MIN, vec[1]);
				Assert::AreEqual(INT64_MAX, vec[2]);
			}
			{
				DefaultAllocator alloc;
				std::vector<int32_t, Allocator<int32_t>> vec(&alloc);
				vec.reserve(4);
				vec.push_back(0);
				vec.push_back(1);
				vec.push_back(2);
				vec.push_back(3);
				vec.push_back(4);

				Assert::AreEqual(5_sz, vec.size());
				for (auto i : {0, 1, 2, 3, 4})
					Assert::AreEqual(i, vec[i]);

				vec.resize(2);
				vec.reserve(42);

				Assert::AreEqual(2_sz, vec.size());
				for (auto i : { 0, 1})
					Assert::AreEqual(i, vec[i]);

				vec.resize(4);

				Assert::AreEqual(4_sz, vec.size());
				for (auto i : { 0, 1 })
					Assert::AreEqual(i, vec[i]);
			}
		}

		TEST_METHOD(DefaultAllocator_String)
		{
			{
				DefaultAllocator alloc;
				std::basic_string<char, std::char_traits<char>, Allocator<char>> string(&alloc);
				string = "Foo";
				string += " ";
				string += "bar";
				Assert::AreEqual("Foo bar", string.c_str());
			}
			{
				DefaultAllocator alloc;
				std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t>> string(&alloc);
				string = L"Foo";
				string += L" ";
				string += L"bar";
				Assert::AreEqual(L"Foo bar", string.c_str());
			}
		}

		TEST_METHOD(DefaultAllocator_Map)
		{
			using namespace std;
			DefaultAllocator alloc;
			std::map<uint8_t, std::string, std::less<uint8_t>, Allocator<std::pair<const uint8_t, std::string>>> map(&alloc);
			map[2] = "Two";
			map[4] = "Four";
			map[8] = "Eight";
			Assert::AreEqual(3_sz, map.size());
			Assert::AreEqual("Two"s, map[2]);
			Assert::AreEqual("Four"s, map[4]);
			Assert::AreEqual("Eight"s, map[8]);
		}

		TEST_METHOD(DefaultAllocator_List)
		{
			typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t>> String;
			typedef std::list<String, Allocator<String>> List;

			DefaultAllocator alloc;
			List list(&alloc);
			list.push_back(String(L"brown", &alloc));
			list.push_back(String(L"fox", &alloc));
			list.push_back(String(L"jumps", &alloc));
			list.push_back(String(L"over", &alloc));
			list.push_back(String(L"the", &alloc));
			list.push_back(String(L"lazy", &alloc));
			list.emplace_back(L"dog", &alloc);
			list.push_front(String(L"quick", &alloc));
			list.emplace_front(L"The", &alloc);

			for (auto word : { L"The", L"quick", L"brown", L"fox", L"jumps", L"over", L"the", L"lazy", L"dog" })
			{
				Assert::AreEqual(word, list.front().c_str());
				list.pop_front();
			}
		}
	};
}
