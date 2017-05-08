#pragma once
#include "Neat\Types.h"

#include <Windows.h>

namespace Neat::Win
{
	class Time
	{
	public:
		Time();

		void MoveForward(uint16_t minutes);
		void MoveBackward(uint16_t years);

		SYSTEMTIME* operator&();
		const SYSTEMTIME* operator&() const;

	private:
		SYSTEMTIME m_time;
	};

	inline SYSTEMTIME* Time::operator&()
	{
		return &m_time;
	}

	inline const SYSTEMTIME* Time::operator&() const
	{
		return &m_time;
	}
}
