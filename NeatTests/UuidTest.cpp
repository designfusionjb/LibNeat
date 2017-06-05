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

			for (auto word : uuid)
				Assert::IsTrue(0 == word);

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
				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					GUID guid;
					::CoCreateGuid(&guid);
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"%u ::CoCreateGuid calls took %Iu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
			UuidGenerator generator;
			volatile auto accumulator = 0;
			{
				const auto start = steady_clock::now();
				for (auto i = 0; i < count; i++)
				{
					auto uuid = generator.Generate();
					accumulator += uuid.GetVersion();
				}
				const auto end = steady_clock::now();
				const auto duration = duration_cast<microseconds>(end - start).count();
				const auto message = Utf16::Format(
					L"%u Uuid::Generate calls took %Iu microseconds",
					count,
					duration);
				Logger::WriteMessage(message);
			}
		}
	};
}