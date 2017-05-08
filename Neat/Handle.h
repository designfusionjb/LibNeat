#pragma once

namespace Neat
{
	template <typename T>
	struct HandleTraits
	{
		static bool IsValid(T handle);
		static void Finalize(T handle);
	};

	template <typename T, typename Traits = HandleTraits<T>>
	class HandleT
	{
	public:
		HandleT();
		HandleT(T handle);
		HandleT(HandleT&& other);
		~HandleT();

		bool IsNull() const;
		bool IsValid() const;

		void Close();
		void Attach(T handle);

		T Detach();
		operator T() const;

		T* operator&();
		const T* operator&() const;

		HandleT& operator=(T handle);
		HandleT& operator=(HandleT&& other);

	protected:
		T m_handle;
	};

	template <typename T, typename Traits>
	HandleT<T, Traits>::HandleT()
	{
		memset(&m_handle, 0, sizeof(m_handle));
	}

	template <typename T, typename Traits>
	HandleT<T, Traits>::HandleT(T handle) :
		m_handle(handle)
	{
	}

	template <typename T, typename Traits>
	HandleT<T, Traits>::HandleT(HandleT&& other) :
		m_handle(other.m_handle)
	{
		memset(&other.m_handle, 0, sizeof(m_handle));
	}

	template <typename T, typename Traits>
	HandleT<T, Traits>::~HandleT()
	{
		Close();
	}

	template <typename T, typename Traits>
	bool HandleT<T, Traits>::IsNull() const
	{
		static const HandleT<T, Traits> s_null;
		if (0 == memcmp(&m_handle, &s_null.m_handle, sizeof(m_handle)))
			return true;

		return false;
	}

	template <typename T, typename Traits>
	bool HandleT<T, Traits>::IsValid() const
	{
		return Traits::IsValid(m_handle);
	}

	template <typename T, typename Traits>
	void HandleT<T, Traits>::Close()
	{
		if (!IsNull())
		{
			Traits::Finalize(m_handle);
			memset(&m_handle, 0, sizeof(m_handle));
		}
	}

	template <typename T, typename Traits>
	void HandleT<T, Traits>::Attach(T handle)
	{
		Close();
		m_handle = handle;
	}

	template <typename T, typename Traits>
	T HandleT<T, Traits>::Detach()
	{
		T handle = m_handle;
		memset(&m_handle, 0, sizeof(m_handle));
		return handle;
	}

	template <typename T, typename Traits>
	HandleT<T, Traits>::operator T() const
	{
		return m_handle;
	}

	template <typename T, typename Traits>
	T* HandleT<T, Traits>::operator&()
	{
		return &m_handle;
	}

	template <typename T, typename Traits>
	const T* HandleT<T, Traits>::operator&() const
	{
		return &m_handle;
	}

	template <typename T, typename Traits>
	HandleT<T, Traits>& HandleT<T, Traits>::operator=(T handle)
	{
		Attach(handle);
		return *this;
	}

	template <typename T, typename Traits>
	HandleT<T, Traits>& HandleT<T, Traits>::operator=(HandleT&& other)
	{
		Attach(other.Detach());
		return *this;
	}
}

