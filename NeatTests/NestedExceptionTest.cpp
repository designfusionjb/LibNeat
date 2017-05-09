#include "CppUnitTest.h"

#include <Neat\Exception.h>

#include <exception>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat
{
	TEST_CLASS(NestedExceptionTest)
	{
	public:
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