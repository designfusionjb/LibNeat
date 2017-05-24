#pragma once
#include "Neat\Allocator.h"

namespace Neat
{
	template <size_t size>
	class StackAllocator : public IAllocator
	{
	public:
		StackAllocator();

		byte_t* Allocate(size_t bytes) override;
		void Deallocate(byte_t* p, size_t bytes) override;
		bool Owns(byte_t* p, size_t bytes) override;

		size_t GetCapacity() const;

	private:
		byte_t m_buffer[size];
		byte_t* m_next;
	};

	template <size_t size>
	StackAllocator<size>::StackAllocator()
		: m_next(m_buffer + size)
	{
		memset(m_buffer, 0, size);
	}

	template <size_t size>
	byte_t* StackAllocator<size>::Allocate(size_t bytes)
	{
		const auto capacity = GetCapacity();
		if (bytes > capacity)
			return nullptr;
		m_next -= bytes;
		return m_next;
	}

	template <size_t size>
	void StackAllocator<size>::Deallocate(byte_t* p, size_t bytes)
	{
	}

	template <size_t size>
	bool StackAllocator<size>::Owns(byte_t* p, size_t bytes)
	{
		return p >= m_buffer && (p + bytes) <= (m_buffer + size);
	}

	template <size_t size>
	size_t StackAllocator<size>::GetCapacity() const
	{
		return static_cast<size_t>(m_next - m_buffer);
	}
}
