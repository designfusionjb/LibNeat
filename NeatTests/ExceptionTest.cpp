#include "CppUnitTest.h"

#include <Neat\Exception.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{
	TEST_CLASS(ExceptionTest)
	{
	public:
		TEST_METHOD(Exception_Basic)
		{
			try
			{
				throw Exception("Some");
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual("Exception: Some", ex.what());
			}
		}

		TEST_METHOD(Exception_Advanced)
		{
			try
			{
				throw Exception("Some");
			}
			catch (const Exception& ex)
			{
				Assert::AreEqual("Some", ex.GetErrorMessage());

				// Check subsequent calls to what() and GetErrorMessage()
				Assert::AreEqual("Exception: Some", ex.what());
				Assert::AreEqual("Some", ex.GetErrorMessage());
			}
		}

		TEST_METHOD(ExceptionWithCode_Basic)
		{
			try
			{
				throw ExceptionWithCode(0x12345678);
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual("ExceptionWithCode: 0x12345678", ex.what());
			}
		}

		TEST_METHOD(ExceptionWithCode_Advanced)
		{
			try
			{
				throw ExceptionWithCode(0x12345678);
			}
			catch (const ExceptionWithCode& ex)
			{
				Assert::AreEqual("0x12345678", ex.GetErrorMessage());
				Assert::AreEqual(0x12345678u, ex.GetErrorCode());

				// Check subsequent calls to what() and GetErrorMessage().
				Assert::AreEqual("ExceptionWithCode: 0x12345678", ex.what());
				Assert::AreEqual("0x12345678", ex.GetErrorMessage());
			}
		}
	};
}