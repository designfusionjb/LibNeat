#pragma once
#include "Neat\Types.h"

#include <array>

namespace Neat
{
	class Exception : public std::exception
	{
		typedef std::exception Base;

	public:
		explicit Exception(const char* const message);
		Exception(const char name[], const char* const message);

		const char* what() const override;
		virtual const char* GetErrorMessage() const;

	protected:
		mutable std::array<char, 256> m_message;
		mutable std::array<char, 256> m_buffer;
	};

	class ExceptionWithCode : public Exception
	{
		typedef Exception Base;

	public:
		explicit ExceptionWithCode(const uint32_t errorCode);
		ExceptionWithCode(const char name[], const uint32_t errorCode);

		uint32_t GetErrorCode() const;
		const char* GetErrorMessage() const override;

	protected:
		uint32_t m_errorCode;
	};

	inline uint32_t ExceptionWithCode::GetErrorCode() const
	{
		return m_errorCode;
	}
}