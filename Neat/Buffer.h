#pragma once
#include "Neat\Types.h"
#include "Neat\Allocator.h"

#include <type_traits>

namespace Neat
{
	class IBuffer
	{
	public:
		virtual const byte_t* GetBuffer() const = 0;
		virtual byte_t* GetBuffer() = 0;

		// Returns size in bytes
		virtual size_t GetSize() const = 0;
		virtual bool IsEmpty() const = 0;
	};

	template <typename T>
	class BufferT : public IBuffer
	{
		static_assert(std::is_trivial<T>::value, "Only trivial types are allowed!");
		// For non trivial types use std::vector to ensure constructors are called.

	public:
		typedef T Element;

		explicit BufferT(IAllocator* allocator);
		// Accepts size in bytes
		explicit BufferT(size_t size = 0, IAllocator* allocator = nullptr);
		// Accepts size in bytes
		BufferT(const T* buffer, size_t size, IAllocator* allocator = nullptr);
		BufferT(const BufferT& other);
		BufferT(BufferT&& other);
		~BufferT();

		void Allocate(size_t size);
		void Free();

		BufferT& Append(const T* buffer, size_t size);

		IAllocator* GetAllocator() const;

		operator const T*() const;
		operator T*();

		const byte_t* GetBuffer() const override;
		byte_t* GetBuffer() override;

		// Returns size in bytes
		size_t GetSize() const override;
		bool IsEmpty() const override;

		BufferT& operator=(const BufferT& other);
		BufferT& operator=(BufferT&& other);

		const T* operator->() const;
		T* operator->();

	protected:
		void CopyFrom(const BufferT& other);
		void MoveFrom(BufferT& other);
		void DoAllocate(size_t size);
		
	protected:
		IAllocator* m_allocator;
		T* m_buffer;
		size_t m_size;

	private:
		friend void swap(BufferT& left, BufferT& right)
		{
			using std::swap;
			swap(left.m_allocator, right.m_allocator);
			swap(left.m_buffer, right.m_buffer);
			swap(left.m_size, right.m_size);
		}

		friend bool operator==(const BufferT& left, const T* right)
		{
			if (nullptr == right)
				return 0 == left.m_size;

			if (0 == left.m_size)
				return nullptr == right;

			return 0 == memcmp(reinterpret_cast<const byte_t*>(right), left.GetBuffer(), left.m_size);
		}

		friend bool operator!=(const BufferT& left, const T* right)
		{
			return !operator==(left, right);
		}

		friend bool operator==(const T* left, const BufferT& right)
		{
			return operator==(right, left);
		}

		friend bool operator!=(const byte_t* left, const BufferT& right)
		{
			return !operator==(left, right);
		}

		friend bool operator==(const BufferT& left, const BufferT& right)
		{
			if (left.m_size != right.m_size)
				return false;

			return 0 == memcmp(left.m_buffer, right.m_buffer, left.m_size);
		}

		friend bool operator!=(const BufferT& left, const BufferT& right)
		{
			return !operator==(left, right);
		}
	};

	template <typename T>
	BufferT<T>::BufferT(IAllocator* allocator) :
		m_allocator(allocator),
		m_buffer(nullptr),
		m_size(0)
	{
	}

	template <typename T>
	BufferT<T>::BufferT(size_t size, IAllocator* allocator) :
		m_allocator(allocator),
		m_buffer(nullptr),
		m_size(0)
	{
		if (size > 0)
			DoAllocate(size);

		if (m_buffer)
			memset(m_buffer, 0, m_size);
	}

	template <typename T>
	BufferT<T>::BufferT(const T* buffer, size_t size, IAllocator* allocator) :
		m_allocator(allocator),
		m_buffer(nullptr),
		m_size(0)
	{
		if (size > 0)
			DoAllocate(size);

		if (m_buffer)
			memcpy(m_buffer, buffer, m_size);
	}

	template <typename T>
	BufferT<T>::BufferT(const BufferT& other) :
		m_allocator(nullptr),
		m_buffer(nullptr),
		m_size(0)
	{
		CopyFrom(other);
	}

	template <typename T>
	BufferT<T>::BufferT(BufferT&& other)
	{
		MoveFrom(other);
	}

	template <typename T>
	BufferT<T>::~BufferT()
	{
		Free();
	}

	template <typename T>
	void BufferT<T>::Allocate(size_t size)
	{
		Free();

		if (size > 0)
			DoAllocate(size);

		if (m_buffer)
			memset(m_buffer, 0, m_size);
	}

	template <typename T>
	void BufferT<T>::Free()
	{
		if (m_size > 0)
		{
			const auto p = reinterpret_cast<byte_t*>(m_buffer);
			m_allocator ? m_allocator->Deallocate(p, m_size) : delete[] p;
			m_buffer = nullptr;
			m_size = 0;
		}
	}

	template <typename T>
	BufferT<T>& BufferT<T>::Append(const T* buffer, size_t size)
	{
		BufferT other(m_size + size, m_allocator);
		memcpy_s(other.m_buffer, other.m_size, m_buffer, m_size);
		const auto p = reinterpret_cast<byte_t*>(other.m_buffer);
		memcpy_s(p + m_size, other.m_size - m_size, buffer, size);
		swap(*this, other);
		return *this;
	}

	template <typename T>
	IAllocator* BufferT<T>::GetAllocator() const
	{
		return m_allocator;
	}

	template <typename T>
	BufferT<T>::operator const T*() const
	{
		return m_buffer;
	}

	template <typename T>
	BufferT<T>::operator T*()
	{
		return m_buffer;
	}

	template <typename T>
	const byte_t* BufferT<T>::GetBuffer() const
	{
		return reinterpret_cast<const byte_t*>(m_buffer);
	}

	template <typename T>
	byte_t* BufferT<T>::GetBuffer()
	{
		return reinterpret_cast<byte_t*>(m_buffer);
	}

	template <typename T>
	size_t BufferT<T>::GetSize() const
	{
		return m_size;
	}

	template <typename T>
	bool BufferT<T>::IsEmpty() const
	{
		return 0 == m_size;
	}

	template <typename T>
	BufferT<T>& BufferT<T>::operator=(const BufferT<T>& other)
	{
		if (this != &other)
		{
			if (m_size == other.m_size)
			{
				memcpy(m_buffer, other.m_buffer, m_size);
			}
			else
			{
				Free(); //TODO: introduce capacity, to avoid unnecessary allocations
				CopyFrom(other);
			}
		}
		return *this;
	}

	template <typename T>
	BufferT<T>& BufferT<T>::operator=(BufferT<T>&& other)
	{
		if (this != &other)
		{
			Free();
			MoveFrom(other);
		}
		return *this;
	}

	template <typename T>
	const T* BufferT<T>::operator->() const
	{
		return m_buffer;
	}

	template <typename T>
	T* BufferT<T>::operator->()
	{
		return m_buffer;
	}

	template <typename T>
	void BufferT<T>::CopyFrom(const BufferT& other)
	{
		if (other.m_size > 0)
		{
			DoAllocate(other.m_size);
			memcpy(m_buffer, other.m_buffer, m_size);
		}
	}

	template <typename T>
	void BufferT<T>::MoveFrom(BufferT& other)
	{
		m_allocator = other.m_allocator;
		m_buffer = other.m_buffer;
		m_size = other.m_size;

		other.m_allocator = nullptr;
		other.m_buffer = nullptr;
		other.m_size = 0;
	}

	template <typename T>
	void BufferT<T>::DoAllocate(size_t size)
	{
		const auto p = m_allocator ? m_allocator->Allocate(size) : new byte_t[size];
		if (p)
		{
			m_buffer = reinterpret_cast<T*>(p);
			m_size = size;
		}
		else
		{
			m_buffer = nullptr;
			m_size = 0;
		}
	}

	typedef BufferT<byte_t> Buffer;
}