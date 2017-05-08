#include "Neat\Win\Exception.h"
#include "Neat\Win\Handle.h"

#include <cstdio>
#include <Windows.h>

namespace Neat::Win
{
	//
	// Structured exception wrapper
	//

	StructuredException::StructuredException(uint32_t errorCode, size_t address, bool continuable) :
		Base("StructuredException", errorCode),
		m_address(address),
		m_continuable(continuable)
	{
	}

	StructuredException::~StructuredException()
	{
		if (!m_continuable)
		{
			// TODO: Add debug output here
			::ExitProcess(m_errorCode);
		}
	}

	const char* StructuredException::GetErrorMessage() const
	{
		snprintf(
			&m_message[0],
			m_message.size(),
			"0x%08X at 0x%08IX (%s)",
			m_errorCode,
			m_address,
			m_continuable ? "continuable" : "non-continuable");

		return &m_message[0];
	}

	static void SeTranslator(uint32_t errorCode, PEXCEPTION_POINTERS pointers)
	{
		size_t address = 0;
		bool continuable = true;
		if (pointers && pointers->ExceptionRecord)
		{
			address = (size_t)pointers->ExceptionRecord->ExceptionAddress;
			if (pointers->ExceptionRecord->ExceptionFlags & EXCEPTION_NONCONTINUABLE)
			{
				continuable = false;
			}
		}
		throw StructuredException(errorCode, address, continuable);
	}

	void StructuredException::EnableInThisThread()
	{
		_set_se_translator(SeTranslator);
	}

	//
	// Win32 error wrapper
	//

	Win32Exception::Win32Exception(uint32_t errorCode) :
		ExceptionWithCode("Win32Exception", errorCode)
	{
	}

	Win32Exception::Win32Exception(const char name[], uint32_t errorCode) :
		ExceptionWithCode(name, errorCode)
	{
	}

	const char* Win32Exception::GetErrorMessage() const
	{
		DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM;
		ModuleHandle source;

		// WinInet errors
		if (12000 <= m_errorCode && m_errorCode < 13000)
		{
			flags = FORMAT_MESSAGE_FROM_HMODULE;
			source = ::LoadLibraryA("wininet.dll");
		}

		const auto end = ::FormatMessageA(
			flags,
			source,
			m_errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			&m_message[0],
			(DWORD)m_message.size(),
			nullptr);

		if (0 == end)
			return ExceptionWithCode::GetErrorMessage();

		for (auto& ch : m_message)
		{
			if (ch == '\0')
				break;
			if (ch == '\r' || ch == '\n')
				ch = '\0';
		}

		return &m_message[0];
	}

	//
	// Last error wrapper
	//

	LastErrorException::LastErrorException() :
		Win32Exception(::GetLastError())
	{
	}
}