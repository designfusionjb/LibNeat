#pragma once
#include "Neat\Allocator.h"

namespace Neat
{
	class DefaultAllocator : public IAllocator
	{
	public:
		DefaultAllocator();

		byte_t* Allocate(size_t bytes) override;
		void Deallocate(byte_t* p, size_t bytes) override;
		bool Owns(byte_t* p, size_t bytes) override;
	};

	inline DefaultAllocator::DefaultAllocator()
	{
	}

	inline byte_t* DefaultAllocator::Allocate(size_t bytes)
	{
		return new byte_t[bytes];
	}

	inline void DefaultAllocator::Deallocate(byte_t* p, size_t bytes)
	{
		operator delete[](p, bytes);
	}

	inline bool DefaultAllocator::Owns(byte_t* p, size_t bytes)
	{
		return true;
	}
}
