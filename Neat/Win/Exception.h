#pragma once
#include "Neat\Exception.h"

namespace Neat::Win
{
	class StructuredException : public ExceptionWithCode
	{
		typedef ExceptionWithCode Base;

	public:
		StructuredException(
			uint32_t errorCode,
			size_t address,
			bool continuable);
		~StructuredException();

		const char* GetErrorMessage() const override;

		static void EnableInThisThread();

	protected:
		size_t m_address;
		bool m_continuable;
	};

	class Win32Exception : public ExceptionWithCode
	{
		typedef ExceptionWithCode Base;

	public:
		explicit Win32Exception(uint32_t errorCode);
		Win32Exception(const char name[], uint32_t errorCode);

		const char* GetErrorMessage() const override;
	};

	class LastErrorException : public Win32Exception
	{
		typedef Win32Exception Base;

	public:
		LastErrorException();
	};
}
