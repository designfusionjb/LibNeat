#include "CppUnitTest.h"

#include <Neat\Types.h>
#include <Neat\Uuid.h>
#include <Neat\Convert.h>

#include <chrono>

#include <Objbase.h>
#pragma comment (lib, "Ole32.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{
	TEST_CLASS(UuidTest)
	{
	public:
		TEST_METHOD(Uuid_Basic)
		{
			Uuid uuid;

			Assert::AreEqual(16_sz, Uuid::SizeInBytes());
			Assert::AreEqual(16_sz, sizeof(uuid.m_raw));

			for (auto dword : uuid)
				Assert::IsTrue(0 == dword);

			Assert::AreEqual<uint32_t>(0, uuid.GetVersion());
			Assert::AreEqual<uint32_t>(0, uuid.GetVariant());

			uuid.SetVersion(Uuid::Version());
			uuid.SetVariant(Uuid::Variant());

			Assert::AreEqual<uint32_t>(Uuid::Version(), uuid.GetVersion());
			Assert::AreEqual<uint32_t>(Uuid::Variant(), uuid.GetVariant());
		}

		TEST_METHOD(Uuid_Generate)
		{
			// Version and variant check
			{
				auto uuid = UuidGenerator().Generate();
				Assert::AreEqual<uint32_t>(4, uuid.GetVersion());
				Assert::AreEqual<uint32_t>(0b10, uuid.GetVariant());
			}

			// Perfomance comparison with ::CoCreateGuid

			using namespace std::chrono;
			const auto count = 100;
			{
				GUID guid;

				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					::CoCreateGuid(&guid);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u ::CoCreateGuid() calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			{
				UuidGenerator generator;

				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					volatile auto uuid = generator.Generate();
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"# %u UuidGenerator::Generate() calls took %llu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			Logger::WriteMessage(L"#");
		}

		TEST_METHOD(Uuid_Init)
		{
			Uuid uuid = { 0xa1, 0xeb, 0x53, 0xe9, 0xc8, 0xef, 0xf7, 0x45, 0x86, 0xdd, 0x01, 0xef, 0x20, 0x1f, 0xe2, 0x57, 0x00 };

			size_t i = 0;
			for (auto byte : { 0xa1, 0xeb, 0x53, 0xe9, 0xc8, 0xef, 0xf7, 0x45, 0x86, 0xdd, 0x01, 0xef, 0x20, 0x1f, 0xe2, 0x57 })
				Assert::AreEqual<uint32_t>(byte, uuid.m_raw[i++]);
		}
	};
}