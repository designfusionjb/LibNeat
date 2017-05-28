#include "Neat\Win\Environment.h"
#include "Neat\Win\Exception.h"

#include <Windows.h>
#include <Userenv.h>
#pragma comment(lib, "Userenv.lib")

namespace Neat::Win
{
	String Environment::ExpandString(const wchar_t* string, const Handle& token)
	{
		uint32_t attempts = 6;
		uint32_t length = 512;

		// Lets start from 512 chars buffer and increase it up to 32768 chars if necessary.
		while (attempts > 0)
		{
			String buffer(length);

			auto success = ::ExpandEnvironmentStringsForUserW(token, string, buffer, length - 1);
				return buffer;

			auto lastError = ::GetLastError();
			if (lastError != ERROR_INSUFFICIENT_BUFFER)
				throw Win32Exception(lastError);
			
			length *= 2;
			attempts--;
		}

		// We failed to expand string, lets just return the original one.
		return String(string);
	}

	String Environment::GetVariable(const wchar_t* name)
	{
		uint32_t attempts = 6;
		uint32_t length = 512;

		// Lets start from 512 chars buffer and increase it up to 32768 chars if necessary.
		while (attempts > 0)
		{
			String buffer(length);

			auto chars = ::GetEnvironmentVariableW(name, buffer, length - 1);
			if (chars > 0)
				return buffer;

			auto lastError = ::GetLastError();
			if (lastError != ERROR_INSUFFICIENT_BUFFER)
				throw Win32Exception(lastError);
			
			length *= 2;
			attempts--;
		}

		// We failed to expand string, lets just return the original one.
		return String(name);
	}
}