#include "CppUnitTest.h"

#include <Neat\Win\Exception.h>

#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat::Win
{
	TEST_CLASS(WinExceptionTest)
	{
	public:
		TEST_METHOD(StructuredExceptionBasic)
		{
			try
			{
				throw StructuredException(512, 0x12345678, true);
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual("StructuredException: 0x00000200 at 0x12345678 (continuable)", ex.what());
			}
		}

		TEST_METHOD(StructuredExceptionAdvanced)
		{
			try
			{
				throw StructuredException(512, 0x12345678, true);
			}
			catch (const ExceptionWithCode& ex)
			{
				Assert::AreEqual("0x00000200 at 0x12345678 (continuable)", ex.GetErrorMessage());
				Assert::AreEqual(512u, ex.GetErrorCode());

				// Check subsequent calls to what() and GetErrorMessage().
				Assert::AreEqual("StructuredException: 0x00000200 at 0x12345678 (continuable)", ex.what());
				Assert::AreEqual("0x00000200 at 0x12345678 (continuable)", ex.GetErrorMessage());
			}
		}

		TEST_METHOD(StructuredExceptionEnable)
		{
			StructuredException::EnableInThisThread();
			Assert::ExpectException<StructuredException>([]()
			{
				int* a = nullptr;
				auto b = *a;
				if (b)
					Logger::WriteMessage("Shouldn't get to this line!!!");
			});
		}

		TEST_METHOD(Win32ExceptionBasic)
		{
			try
			{
				throw Win32Exception(ERROR_ACCESS_DENIED);
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual("Win32Exception: Access is denied.", ex.what());
			}
		}

		TEST_METHOD(Win32ExceptionAdvanced)
		{
			try
			{
				throw Win32Exception(0x12345678);
			}
			catch (const ExceptionWithCode& ex)
			{
				Assert::AreEqual("0x12345678", ex.GetErrorMessage());
				Assert::AreEqual(0x12345678u, ex.GetErrorCode());

				// Check subsequent calls to what() and GetErrorMessage().
				Assert::AreEqual("Win32Exception: 0x12345678", ex.what());
				Assert::AreEqual("0x12345678", ex.GetErrorMessage());
			}
		}

		TEST_METHOD(LastErrorExceptionBasic)
		{
			try
			{
				::SetLastError(ERROR_FILE_NOT_FOUND);
				throw LastErrorException();
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual("Win32Exception: The system cannot find the file specified.", ex.what());
			}
		}

		TEST_METHOD(LastErrorExceptionAdvanced)
		{
			try
			{
				::SetLastError(ERROR_PATH_NOT_FOUND);
				throw LastErrorException();
			}
			catch (const ExceptionWithCode& ex)
			{
				Assert::AreEqual("The system cannot find the path specified.", ex.GetErrorMessage());
				Assert::AreEqual(3u, ex.GetErrorCode());

				// Check subsequent calls to what() and GetErrorMessage().
				Assert::AreEqual("Win32Exception: The system cannot find the path specified.", ex.what());
				Assert::AreEqual("The system cannot find the path specified.", ex.GetErrorMessage());
			}
		}
	};
}