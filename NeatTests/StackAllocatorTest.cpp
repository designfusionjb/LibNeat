#include "IntToString.h"
#include <CppUnitTest.h>

#include <Neat\Types.h>
#include <Neat\StackAllocator.h>

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{
	TEST_CLASS(StackAllocator_Test)
	{
	public:
		TEST_METHOD(StackAllocator_Basic)
		{
			StackAllocator<32> alloc;
			for (auto i : {1, 2, 3, 4})
			{
				auto p = alloc.Allocate(8);
				Assert::IsNotNull(p);
				Assert::IsTrue(alloc.Owns(p, 8));
			}
			Assert::AreEqual(0_sz, alloc.GetCapacity());
		}

		TEST_METHOD(StackAllocator_Vector)
		{
			StackAllocator<32> alloc;
			std::vector<uint16_t, Allocator<uint16_t>> vec(&alloc);
			vec.push_back(1);
			vec.push_back(2);
			vec.reserve(4);
			vec.push_back(3);
			vec.push_back(4);

			const auto capacity = alloc.GetCapacity();
			vec.clear();
		}
	};
}
