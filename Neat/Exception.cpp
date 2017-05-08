#include "Neat\Exception.h"

#include <stdio.h>  
#include <string.h>

namespace Neat
{
	//
	// Base exception class
	//

	Exception::Exception(const char* const message) :
		Base("Exception", 1)
	{
		strcpy_s(&m_message[0], m_message.size(), message);
	}

	Exception::Exception(const char name[], const char* const message) :
		Base(name, 1)
	{
		strcpy_s(&m_message[0], m_message.size(), message);
	}

	const char* Exception::what() const
	{
		const auto name = Base::what();
		const auto message = GetErrorMessage();
		sprintf_s(&m_buffer[0], m_buffer.size(), "%s: %s", name, message);
		return &m_buffer[0];
	}

	const char* Exception::GetErrorMessage() const
	{
		return &m_message[0];
	}

	//
	// Exception with error code
	//

	ExceptionWithCode::ExceptionWithCode(const uint32_t errorCode) :
		Base("ExceptionWithCode", ""),
		m_errorCode(errorCode)
	{
	}

	ExceptionWithCode::ExceptionWithCode(const char name[], const uint32_t errorCode) :
		Base(name, ""),
		m_errorCode(errorCode)
	{
	}

	const char* ExceptionWithCode::GetErrorMessage() const
	{
		sprintf_s(&m_message[0], m_message.size(), "0x%08X", m_errorCode);
		return &m_message[0];
	}
}