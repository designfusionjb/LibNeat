#include "IntToString.h"
#include <CppUnitTest.h>

#include <Neat\Types.h>
#include <Neat\FallbackAllocator.h>
#include <Neat\MallocAllocator.h>
#include <Neat\StackAllocator.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{
	TEST_CLASS(FallbackAllocatorTest)
	{
	public:
		TEST_METHOD(FallbackAllocator_Basic)
		{
			StackAllocator<16> primary;
			MallocAllocator fallback;
			FallbackAllocator alloc(&primary, &fallback);
			for (auto i : { 1, 2, 3, 4 })
			{
				auto p = alloc.Allocate(4);
				Assert::IsNotNull(p);
				Assert::IsTrue(alloc.Owns(p, 4));
				Assert::IsTrue(primary.Owns(p, 4));
				*p = i;
			}
			Assert::AreEqual(0_sz, primary.GetCapacity());

			auto p = alloc.Allocate(4);
			Assert::IsNotNull(p);
			Assert::IsTrue(alloc.Owns(p, 4));
			Assert::IsTrue(fallback.Owns(p, 4));
			Assert::IsFalse(primary.Owns(p, 4));
			*p = 0;
			alloc.Deallocate(p, 4);
		}

		TEST_METHOD(FallbackAllocator_Vector)
		{
			StackAllocator<16> primary;
			MallocAllocator fallback;
			FallbackAllocator alloc(&primary, &fallback);
			std::vector<int32_t, Allocator<int32_t>> vec(&alloc);
			vec.push_back(1);
			vec.push_back(2);
			vec.push_back(3);
			vec.push_back(4);
			vec.push_back(5);
		}
	};
}
