#pragma once
#include "Neat\Allocator.h"

namespace Neat
{
	class FallbackAllocator : public IAllocator
	{
	public:
		FallbackAllocator(
			IAllocator* primary,
			IAllocator* fallback);

		byte_t* Allocate(size_t bytes) override;
		void Deallocate(byte_t* p, size_t bytes) override;
		bool Owns(byte_t* p, size_t bytes) override;

	private:
		IAllocator* m_primary;
		IAllocator* m_fallback;
	};

	inline FallbackAllocator::FallbackAllocator(
		IAllocator* primary,
		IAllocator* fallback) :
		m_primary(primary),
		m_fallback(fallback)
	{
	}

	inline byte_t* FallbackAllocator::Allocate(size_t bytes)
	{
		auto p = m_primary->Allocate(bytes);
		if (!p)
			p = m_fallback->Allocate(bytes);
		return p;
	}

	inline void FallbackAllocator::Deallocate(byte_t* p, size_t bytes)
	{
		if (m_primary->Owns(p, bytes))
			m_primary->Deallocate(p, bytes);
		else
			m_fallback->Deallocate(p, bytes);
	}

	inline bool FallbackAllocator::Owns(byte_t* p, size_t bytes)
	{
		return m_primary->Owns(p, bytes) ||
			m_fallback->Owns(p, bytes);
	}
}
