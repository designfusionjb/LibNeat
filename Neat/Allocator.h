#pragma once
#include "Neat\Types.h"

namespace Neat
{
	class IAllocator
	{
	public:
		virtual byte_t* Allocate(size_t bytes) = 0;
		virtual void Deallocate(byte_t* p, size_t bytes) = 0;
		virtual bool Owns(byte_t* p, size_t bytes) = 0;
	};

	template <class T>
	class Allocator
	{
	public:
		typedef T value_type;

		Allocator(IAllocator* allocator) noexcept
			: m_allocator(allocator)
		{
		}

		Allocator(Allocator&& other) noexcept
			: m_allocator(other.m_allocator)
		{
			other.m_allocator = nullptr;
		}

		Allocator(const Allocator& other) noexcept
			: m_allocator(other.m_allocator)
		{
		}

		template <class U>
		Allocator(Allocator<U>&& other) noexcept
			: m_allocator(other.m_allocator)
		{
			other.m_allocator = nullptr;
		}

		template <class U>
		Allocator(const Allocator<U>& other) noexcept
			: m_allocator(other.m_allocator)
		{
		}

		~Allocator() noexcept
		{
		}

		Allocator& operator=(Allocator&& other) noexcept
		{
			m_allocator = other.m_allocator;
			other.m_allocator = nullptr;
			return *this;
		}

		Allocator& operator=(const Allocator& other) noexcept
		{
			m_allocator = other.m_allocator;
			return *this;
		}

		template <class U>
		Allocator& operator=(Allocator<U>&& other) noexcept
		{
			m_allocator = other.m_allocator;
			other.m_allocator = nullptr;
			return *this;
		}

		template <class U>
		Allocator& operator=(const Allocator<U>& other) noexcept
		{
			m_allocator = other.m_allocator;
			return *this;
		}

		T* allocate(size_t n)
		{
			if (!m_allocator)
				return nullptr;
			auto p = m_allocator->Allocate(sizeof(T) * n);
			return reinterpret_cast<T*>(p);
		}

		void deallocate(T* p, size_t n)
		{
			if (!m_allocator)
				return;
			m_allocator->Deallocate(reinterpret_cast<byte_t*>(p), sizeof(T) * n);
		}

	public:
		IAllocator* m_allocator;
	};
}
