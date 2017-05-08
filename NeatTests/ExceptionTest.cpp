#include "CppUnitTest.h"

#include <Neat\Exception.h>
#include <Neat\Win\Exception.h>

#include <exception>
#include <string>

#include <Windows.h>

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

		TEST_METHOD(StructuredExceptionBasic)
		{
			try
			{
				throw Win::StructuredException(512, 0x12345678, true);
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
				throw Win::StructuredException(512, 0x12345678, true);
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
			Win::StructuredException::EnableInThisThread();
			Assert::ExpectException<Win::StructuredException>([]()
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
				throw Win::Win32Exception(ERROR_ACCESS_DENIED);
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
				throw Win::Win32Exception(0x12345678);
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
				throw Win::LastErrorException();
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
				throw Win::LastErrorException();
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

		//
		// Class with a nested exception support
		//
		class ExceptionWithNested : public Exception, public std::nested_exception
		{
		public:
			ExceptionWithNested(const char* message) :
				Exception("ExceptionWithNested", message)
			{
			}
		};

		void ThrowNested(uint32_t depth, uint32_t level = 0)
		{
			static const char message[8][8] = { "Level 0", "Level 1", "Level 2", "Level 3", "Level 4", "Level 5", "Level 6", "Level 7" };

			//
			// Recurse to the bottom and throw there.
			// Catch and wrap exceptions on the way up.
			//

			if (level == depth)
			{
				throw Exception(message[level]);
				// Also will work
				throw std::exception(message[level]);
			}

			try
			{
				ThrowNested(depth, level + 1);
			}
			catch (...)
			{
				throw ExceptionWithNested(message[level]);
				// Also will work
				std::throw_with_nested(std::exception(message[level]));
			}
		}

		void HandleNested(const std::exception& ex, std::string pre = "")
		{
			try
			{
				const auto message = pre + ex.what();
				Logger::WriteMessage(message.c_str());
				std::rethrow_if_nested(ex);
			}
			catch (const std::exception& ex)
			{
				HandleNested(ex, pre + '>');
			}
		}

		TEST_METHOD(NestedExceptions)
		{
			try
			{
				ThrowNested(3);
			}
			catch (const std::exception& ex)
			{
				HandleNested(ex);
			}
		}
	};
}