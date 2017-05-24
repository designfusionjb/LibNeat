#pragma once
#include "Neat\Allocator.h"

namespace Neat
{
	class MallocAllocator : public IAllocator
	{
	public:
		MallocAllocator();

		byte_t* Allocate(size_t bytes) override;
		void Deallocate(byte_t* p, size_t bytes) override;
		bool Owns(byte_t* p, size_t bytes) override;
	};

	inline MallocAllocator::MallocAllocator()
	{
	}

	inline byte_t* MallocAllocator::Allocate(size_t bytes)
	{
		return static_cast<byte_t*>(::malloc(bytes));
	}

	inline void MallocAllocator::Deallocate(byte_t* p, size_t bytes)
	{
		::free(p);
	}

	inline bool MallocAllocator::Owns(byte_t* p, size_t bytes)
	{
		return true;
	}
}
