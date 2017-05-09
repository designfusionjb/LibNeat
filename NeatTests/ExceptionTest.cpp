#include "CppUnitTest.h"

#include <Neat\Exception.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{
	TEST_CLASS(ExceptionTest)
	{
	public:
		TEST_METHOD(ExceptionBasic)
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

		TEST_METHOD(ExceptionAdvanced)
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

		TEST_METHOD(ExceptionWithCodeBasic)
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

		TEST_METHOD(ExceptionWithCodeAdvanced)
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