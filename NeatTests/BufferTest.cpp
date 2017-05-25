#include <CppUnitTest.h>

#include <Neat\Buffer.h>
#include <Neat\StackAllocator.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{		
	TEST_CLASS(BufferTest)
	{
	public:
		TEST_METHOD(Buffer_Empty)
		{
			Buffer buffer;

			Assert::IsTrue(buffer.IsEmpty());
			Assert::IsTrue(0 == buffer.GetSize());
			Assert::IsNull(buffer.GetBuffer());
		}

		TEST_METHOD(Buffer_Basic)
		{
			BufferT<int32_t> buffer(3);

			Assert::IsFalse(buffer.IsEmpty());
			Assert::IsTrue(3 == buffer.GetSize());
			Assert::IsNotNull(buffer.GetBuffer());

			int32_t* ptr = buffer;
			Assert::IsTrue(ptr == buffer.GetBuffer());
		}

		TEST_METHOD(Buffer_Const)
		{
			const BufferT<wchar_t> buffer(10);

			Assert::IsFalse(buffer.IsEmpty());
			Assert::IsTrue(10 == buffer.GetSize());
			Assert::IsNotNull(buffer.GetBuffer());

			const wchar_t* ptr = buffer;
			Assert::IsTrue(ptr == buffer.GetBuffer());
		}

		TEST_METHOD(Buffer_CopyAssign)
		{
			BufferT<char> empty;
			BufferT<char> buffer("Some", 5);

			Assert::IsNull(empty.GetBuffer());
			Assert::IsNotNull(buffer.GetBuffer());
			Assert::IsTrue(0 == empty.GetSize());
			Assert::IsTrue(5 == buffer.GetSize());

			const auto address = buffer.GetBuffer();
			empty = buffer;

			Assert::IsNotNull(empty.GetBuffer());
			Assert::IsNotNull(buffer.GetBuffer());
			Assert::IsTrue(5 == empty.GetSize());
			Assert::IsTrue(5 == buffer.GetSize());
			Assert::IsFalse(address == empty.GetBuffer());
		}

		TEST_METHOD(Buffer_MoveAssign)
		{
			BufferT<char> empty;
			BufferT<char> buffer("Some", 5);

			Assert::IsNull(empty.GetBuffer());
			Assert::IsNotNull(buffer.GetBuffer());
			Assert::IsTrue(0 == empty.GetSize());
			Assert::IsTrue(5 == buffer.GetSize());

			const auto address = buffer.GetBuffer();
			empty = std::move(buffer);

			Assert::IsNotNull(empty.GetBuffer());
			Assert::IsNull(buffer.GetBuffer());
			Assert::IsTrue(5 == empty.GetSize());
			Assert::IsTrue(0 == buffer.GetSize());
			Assert::IsTrue(address == empty.GetBuffer());
		}

		TEST_METHOD(Buffer_CopyConstruct)
		{
			BufferT<char> buffer("Some", 5);
			BufferT<char> copy(buffer);

			Assert::IsNotNull(buffer.GetBuffer());
			Assert::IsNotNull(copy.GetBuffer());
			Assert::IsTrue(5 == buffer.GetSize());
			Assert::IsTrue(5 == copy.GetSize());
			Assert::IsFalse(buffer.GetBuffer() == copy.GetBuffer());
		}

		TEST_METHOD(Buffer_MoveConstruct)
		{
			BufferT<char> buffer("Some", 5);
			const auto address = buffer.GetBuffer();
			BufferT<char> copy(std::move(buffer));

			Assert::IsNull(buffer.GetBuffer());
			Assert::IsNotNull(copy.GetBuffer());
			Assert::IsTrue(0 == buffer.GetSize());
			Assert::IsTrue(5 == copy.GetSize());
			Assert::IsTrue(address == copy.GetBuffer());
		}

		TEST_METHOD(Buffer_Equality)
		{
			const byte_t expected[] = { 0x4C, 0x00, 0x00, 0x00, 0x01, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
			const auto expectedSize = sizeof(expected);

			Buffer actual;

			Assert::IsTrue(actual == nullptr);
			Assert::IsTrue(nullptr == actual);

			Assert::IsFalse(actual != nullptr);
			Assert::IsFalse(nullptr != actual);

			Assert::IsTrue(actual != expected);
			Assert::IsTrue(expected != actual);

			Assert::IsFalse(actual == expected);
			Assert::IsFalse(expected == actual);

			actual.Append(expected, expectedSize);

			Assert::IsTrue(actual != nullptr);
			Assert::IsTrue(nullptr != actual);

			Assert::IsFalse(actual == nullptr);
			Assert::IsFalse(nullptr == actual);

			Assert::IsTrue(actual == expected);
			Assert::IsTrue(expected == actual);

			Assert::IsFalse(actual != expected);
			Assert::IsFalse(expected != actual);
		}

		TEST_METHOD(Buffer_Append)
		{
			byte_t foo[] = { 0x0F, 0xF0, 0xFF };
			size_t fooSize = sizeof(foo);

			byte_t bar[] = { 0x81, 0x18, 0x42 };
			size_t barSize = sizeof(bar);

			Buffer buffer(foo, fooSize);

			Assert::IsTrue(3 == buffer.GetSize());
			Assert::IsTrue(0x0F == buffer[0]);
			Assert::IsTrue(0xF0 == buffer[1]);
			Assert::IsTrue(0xFF == buffer[2]);

			buffer.Append(bar, barSize);

			Assert::IsTrue(6 == buffer.GetSize());
			Assert::IsTrue(0x0F == buffer[0]);
			Assert::IsTrue(0xF0 == buffer[1]);
			Assert::IsTrue(0xFF == buffer[2]);
			Assert::IsTrue(0x81 == buffer[3]);
			Assert::IsTrue(0x18 == buffer[4]);
			Assert::IsTrue(0x42 == buffer[5]);
		}

		TEST_METHOD(Buffer_AllocateFree)
		{
			BufferT<wchar_t> buffer;

			Assert::IsTrue(buffer.IsEmpty());
			Assert::IsNull(buffer.GetBuffer());
			Assert::AreEqual(0_sz, buffer.GetSize());

			buffer.Allocate(7);

			Assert::IsFalse(buffer.IsEmpty());
			Assert::IsNotNull(buffer.GetBuffer());
			Assert::AreEqual(7_sz, buffer.GetSize());

			buffer.Free();

			Assert::IsTrue(buffer.IsEmpty());
			Assert::IsNull(buffer.GetBuffer());
			Assert::AreEqual(0_sz, buffer.GetSize());
		}

		TEST_METHOD(Buffer_CustomAllocator)
		{
			StackAllocator<20> alloc;
			BufferT<wchar_t> defaultBuffer(L"Foo", 3);

			BufferT<wchar_t> customBuffer(3, &alloc);
			customBuffer = defaultBuffer;
			customBuffer.Append(L"Bar", 4);

			const auto data = customBuffer.GetBuffer();
			const auto size = customBuffer.GetSize();
			Assert::AreEqual(7_sz, size);
			Assert::AreEqual(L"FooBar", data);

			const auto capacity = alloc.GetCapacity();
			Assert::AreEqual(0_sz, capacity);
		}
	};
}