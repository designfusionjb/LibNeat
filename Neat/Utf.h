#pragma once
#include "Neat\Types.h"
#include "Neat\Buffer.h"

#include <exception>
#include <type_traits>
#include <vector>

namespace Neat
{
	template <typename T>
	struct CharTraits
	{
		typedef T CharType;

		static size_t GetLength(const T* string);
		static void Copy(T* buffer, size_t capacity, const T* source);
		static void Copy(T* buffer, size_t capacity, const T* source, size_t length);
		static size_t FormatCount(const T* format, ...);
		static size_t Format(T* buffer, size_t size, const T* format, ...);
		static void ToLower(T* string, size_t size);
		static void ToUpper(T* string, size_t size);
		static T ToLower(const T t);
		static T ToUpper(const T t);
		static bool OneOf(const T t, const T* set);
		static int32_t Compare(const T* left, const T* right);
		static int32_t Compare(const T* left, const T* right, size_t length);
		static const T* Find(const T* string, const T* what);
		static const T* FindLast(const T* string, const T* what);
	};

	template <typename T>
	bool CharTraits<T>::OneOf(const T t, const T* set)
	{
		while (*set)
		{
			if (t == *set)
				return true;
			set++;
		}
		return false;
	}

	template <typename T>
	const T* CharTraits<T>::FindLast(const T* string, const T* what)
	{
		const auto thisLength = GetLength(string);
		const auto whatLength = GetLength(what);
		if (thisLength < whatLength)
			return nullptr;

		const auto diff = thisLength - whatLength;
		auto ptr = string + diff;
		while (ptr >= string)
		{
			if (0 == Compare(ptr, what, whatLength))
				return ptr;
			ptr--;
		}
		return nullptr;
	}

	typedef CharTraits<char> Utf8Traits;
	typedef CharTraits<wchar_t> Utf16Traits;

	//
	// TODO: Consider doing strings immutable (thread safe) and sharing ref-counted buffer.
	// TODO: Consider implementing short string optimization.
	//

	template <typename T, typename Traits = CharTraits<T>>
	class StringT : protected BufferT<T>
	{
		typedef BufferT<T> Base;

	public:
		static const auto End = static_cast<size_t>(-1);

		explicit StringT(IAllocator* allocator = nullptr);
		// Accepts length in code units
		explicit StringT(size_t length, IAllocator* allocator = nullptr);
		// Accepts length in code units
		StringT(const T* string, size_t length = End, IAllocator* allocator = nullptr);
		StringT(const StringT& other);
		StringT(StringT&& other);

		Base::operator const T*;
		Base::operator T*;

		Base::GetAllocator;
		Base::GetBuffer;
		Base::GetSize;
		
		bool IsEmpty() const;
		bool IsEqual(const T* string) const;

		// Returns length in code units
		size_t GetLength() const;

		// Accepts length in code units
		void Reserve(size_t length);
		void Clear();

		StringT& operator=(const T* string);
		StringT& operator=(const StringT& other);
		StringT& operator=(StringT&& other);

		StringT& operator+=(const T* string);
		StringT& operator+=(const T t);

		StringT operator+(const T* string) const;

		bool operator==(const T* string) const;
		bool operator==(const StringT& other) const;

		bool operator!=(const T* string) const;
		bool operator!=(const StringT& other) const;

		bool operator<(const StringT& other) const;

		void Attach(const T* string);
		T* Detach();

		void Append(const T* string, size_t length = End);
		void Append(const T t);

		size_t Find(const T* what, size_t from = 0) const;
		size_t FindLast(const T* what) const;

		void Replace(const T* what, const T* with);
		void Replace(size_t from, const T* with);
		void Replace(size_t from, size_t to, const T* with);

		void ToLower();
		void ToUpper();

		StringT ToLower() const;
		StringT ToUpper() const;

		bool BeginsWith(const T* what) const;
		bool EndsWith(const T* what) const;
		bool Contains(const T* what) const;
		bool Match(const T* wildcard) const;

		StringT Substring(size_t from, size_t length = End) const;

		size_t Split(const T* by, StringT& token, size_t& from) const;
		std::vector<StringT> Split(const T* by) const;

		void Trim(const T* what);
		void TrimLeft(const T* what);
		void TrimRight(const T* what);

		static size_t GetLength(const T* string);

		template <typename... Ts>
		static StringT Format(const T* format, const Ts&... ts);

	protected:
		void CopyFrom(const StringT& other);
		void MoveFrom(StringT& other);

		void DoReserve(size_t length);
		void DoReplace(size_t from, size_t whatLength, const T* with, size_t withLength);

	private:
		friend void swap(StringT& left, StringT& right)
		{
			using std::swap;
			swap(left.m_buffer, right.m_buffer);
			swap(left.m_size, right.m_size);
		}
	};

	template <typename T, typename Traits>
	StringT<T, Traits>::StringT(IAllocator* allocator) :
		Base(allocator)
	{
	}

	template <typename T, typename Traits>
	StringT<T, Traits>::StringT(size_t length, IAllocator* allocator) :
		Base(allocator)
	{
		DoReserve(length);
	}

	template <typename T, typename Traits>
	StringT<T, Traits>::StringT(const T* string, size_t length, IAllocator* allocator) :
		Base(allocator)
	{
		if (string != nullptr)
		{
			length = (length != End) ? length : Traits::GetLength(string);
			DoReserve(length);
			Traits::Copy(m_buffer, m_size / sizeof(T), string, length);
		}
	}

	template <typename T, typename Traits>
	StringT<T, Traits>::StringT(const StringT& other)
	{
		CopyFrom(other);
	}

	template <typename T, typename Traits>
	StringT<T, Traits>::StringT(StringT&& other)
	{
		MoveFrom(other);
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::IsEmpty() const
	{
		return 0 == GetLength();
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::IsEqual(const T* string) const
	{
		return *this == string;
	}

	template <typename T, typename Traits>
	size_t StringT<T, Traits>::GetLength() const
	{
		return Traits::GetLength(m_buffer);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Reserve(size_t length)
	{
		const auto required = (length + 1) * sizeof(T);
		if (required > m_size)
		{
			StringT other(length, m_allocator);
			swap(*this, other);
			if (other.m_size > 0)
				Traits::Copy(m_buffer, m_size / sizeof(T), other.m_buffer);
		}
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Clear()
	{
		Free();
	}

	template <typename T, typename Traits>
	StringT<T, Traits>& StringT<T, Traits>::operator=(const T* string)
	{
		const auto length = Traits::GetLength(string);
		Reserve(length);
		Traits::Copy(m_buffer, m_size / sizeof(T), string);
		return *this;
	}

	template <typename T, typename Traits>
	StringT<T, Traits>& StringT<T, Traits>::operator=(const StringT& other)
	{
		if (this != &other)
		{
			Free();
			CopyFrom(other);
		}
		return *this;
	}

	template <typename T, typename Traits>
	StringT<T, Traits>& StringT<T, Traits>::operator=(StringT&& other)
	{
		if (this != &other)
		{
			Free();
			MoveFrom(other);
		}
		return *this;
	}

	template <typename T, typename Traits>
	StringT<T, Traits>& StringT<T, Traits>::operator+=(const T* string)
	{
		Append(string);
		return *this;
	}

	template <typename T, typename Traits>
	StringT<T, Traits>& StringT<T, Traits>::operator+=(const T t)
	{
		Append(t);
		return *this;
	}

	template <typename T, typename Traits>
	StringT<T, Traits> StringT<T, Traits>::operator+(const T* string) const
	{
		StringT result(*this);
		result.Append(string);
		return result;
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::operator==(const T* string) const
	{
		if (nullptr == m_buffer)
		{
			if (nullptr == string)
				return true;
			return false;
		}
		else
		{
			if (nullptr == string)
				return false;
			return 0 == Traits::Compare(m_buffer, string);
		}
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::operator==(const StringT& other) const
	{
		return operator==(other.operator const T*());
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::operator!=(const T* string) const
	{
		return !operator==(string);
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::operator!=(const StringT& other) const
	{
		return operator!=(other.operator const T*());
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::operator<(const StringT& other) const
	{
		if (nullptr == m_buffer)
		{
			if (nullptr == other.m_buffer)
				return false;
			return true;
		}
		else
		{
			if (nullptr == other.m_buffer)
				return false;
			return 0 > Traits::Compare(m_buffer, other.m_buffer);
		}
	}
	
	template <typename T, typename Traits>
	void StringT<T, Traits>::Attach(const T* string)
	{
		Free();
		const auto length = Traits::GetLength(string);
		m_buffer = string;
		m_size = (length + 1) * sizeof(T);
	}

	template <typename T, typename Traits>
	T* StringT<T, Traits>::Detach()
	{
		const auto string = m_buffer;
		m_buffer = nullptr;
		m_size = 0;
		return string;
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Append(const T* string, size_t length)
	{
		const auto addLength = (length != End) ? length : Traits::GetLength(string);
		if (0 == addLength)
			return;

		const auto oldLength = Traits::GetLength(m_buffer);
		const auto newLength = oldLength + addLength;
		Reserve(newLength);

		const auto buffer = m_buffer + oldLength;
		const auto capacity = m_size / sizeof(T) - oldLength;
		Traits::Copy(buffer, capacity, string, addLength);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Append(const T t)
	{
		const auto length = Traits::GetLength(m_buffer);
		Reserve(length + 1);

		const auto buffer = m_buffer + length;
		const auto capacity = m_size / sizeof(T) - length;
		Traits::Copy(buffer, capacity, &t, 1);
	}

	template <typename T, typename Traits>
	size_t StringT<T, Traits>::Find(const T* what, size_t from = 0) const
	{
		if (nullptr == m_buffer)
		{
			if (nullptr == what)
				return 0;

			if (0 == *what)
				return 0;

			return End;
		}

		if (nullptr == what)
			return 0;

		if (0 == *what)
			return 0;

		if (from >= m_size)
			return End;

		const auto where = Traits::Find(m_buffer + from, what);
		if (nullptr == where)
			return End;

		return where - m_buffer;
	}

	template <typename T, typename Traits>
	size_t StringT<T, Traits>::FindLast(const T* what) const
	{
		if (nullptr == m_buffer)
		{
			if (nullptr == what)
				return 0;

			if (0 == *what)
				return 0;

			return End;
		}

		if (nullptr == what)
			return 0;

		if (0 == *what)
			return 0;

		const auto where = Traits::FindLast(m_buffer, what);
		if (nullptr == where)
			return End;

		return where - m_buffer;
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Replace(const T* what, const T* with)
	{
		if (!what || !with)
			return;

		auto pos = Find(what);
		if (End == pos)
			return;

		const auto whatLength = Traits::GetLength(what);
		const auto withLength = Traits::GetLength(with);

		while (End != pos)
		{
			DoReplace(pos, whatLength, with, withLength);
			pos = Find(what, pos + withLength);
		}
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Replace(size_t from, const T* with)
	{
		const auto thisLength = Traits::GetLength(m_buffer);
		if (from > thisLength)
			return;
		const auto whatLength = thisLength - from;
		const auto withLength = Traits::GetLength(with);
		DoReplace(from, whatLength, with, withLength);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Replace(size_t from, size_t to, const T* with)
	{
		if (from >= m_size)
			throw std::out_of_range("from >= m_size");
		if (to >= m_size)
			throw std::out_of_range("to >= m_size");
		if (to < from)
			throw std::out_of_range("to < from");
		const auto whatLength = to - from;
		const auto withLength = Traits::GetLength(with);
		DoReplace(from, whatLength, with, withLength);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::ToLower()
	{
		if (m_size > 0)
			Traits::ToLower(m_buffer, m_size);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::ToUpper()
	{
		if (m_size > 0)
			Traits::ToUpper(m_buffer, m_size);
	}

	template <typename T, typename Traits>
	StringT<T, Traits> StringT<T, Traits>::ToLower() const
	{
		StringT copy(*this);
		copy.ToLower();
		return copy;
	}

	template <typename T, typename Traits>
	StringT<T, Traits> StringT<T, Traits>::ToUpper() const
	{
		StringT copy(*this);
		copy.ToUpper();
		return copy;
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::BeginsWith(const T* what) const
	{
		if (nullptr == what)
		{
			if (nullptr == m_buffer)
				return true;
		}

		const auto whatLength = Traits::GetLength(what);
		if (0 == whatLength)
			return true;

		return 0 == Traits::Compare(m_buffer, what, whatLength);
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::EndsWith(const T* what) const
	{
		if (nullptr == what)
		{
			if (nullptr == m_buffer)
				return true;
		}

		const auto whatLength = Traits::GetLength(what);
		if (0 == whatLength)
			return true;

		const auto thisLength = Traits::GetLength(m_buffer);
		if (thisLength < whatLength)
			return false;

		const auto pos = thisLength - whatLength;
		return 0 == Traits::Compare(m_buffer + pos, what);
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::Contains(const T* what) const
	{
		return End != Find(what);
	}

	template <typename T, typename Traits>
	bool StringT<T, Traits>::Match(const T* wild) const
	{
		auto equal = [](const T* wild, const T* string)
		{
			if (*wild == *string)
				return true;

			if (*wild == '?')
				return true;

			if (Traits::ToLower(*wild) ==
				Traits::ToLower(*string))
				return true;
			
			return false;
		};

		const T* string = m_buffer;
		if (nullptr == string)
		{
			return nullptr == wild;
		}

		while ((*string) && (*wild != '*'))
		{
			if (!equal(wild, string))
				return false;

			wild++;
			string++;
		}

		const T* cp = nullptr;
		const T* mp = nullptr;

		while (*string)
		{
			if (*wild == '*')
			{
				if (!*++wild)
					return true;

				mp = wild;
				cp = string + 1;
			}
			else if (equal(wild, string))
			{
				wild++;
				string++;
			}
			else
			{
				wild = mp;
				string = cp++;
			}
		}

		while (*wild == '*')
			wild++;

		if (*wild)
			return false;

		return true;
	}

	template <typename T, typename Traits>
	StringT<T, Traits> StringT<T, Traits>::Substring(size_t from, size_t length) const
	{
		const auto capacity = (m_size > 0) ? m_size / sizeof(T) - 1 : 0;
		if (from > capacity)
			throw std::out_of_range("from > capacity");
		length = (length != End) ? length : Traits::GetLength(m_buffer + from);
		if (from + length > capacity)
			length = capacity - from;
		return StringT(m_buffer + from, length);
	}

	template <typename T, typename Traits>
	size_t StringT<T, Traits>::Split(const T* by, StringT& token, size_t& from) const
	{
		const auto capacity = (m_size > 0) ? m_size / sizeof(T) - 1 : 0;
		if (from >= capacity)
			return End;

		const auto pos = Find(by, from);
		if (End != pos)
		{
			token = Substring(from, pos - from);
			from += token.GetLength() + Traits::GetLength(by);
			return 0;
		}
		else
		{
			token = Substring(from);
			from = m_size;
			return 0;
		}
	}

	template <typename T, typename Traits>
	std::vector<StringT<T, Traits>> StringT<T, Traits>::Split(const T* by) const
	{
		std::vector<StringT> tokenList;
		StringT token;
		size_t from = 0;

		auto pos = Split(by, token, from);
		while (-1 != pos)
		{
			if (!token.IsEmpty())
				tokenList.push_back(token);
			pos = Split(by, token, from);
		}
		return tokenList;
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::Trim(const T* what)
	{
		TrimLeft(what);
		TrimRight(what);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::TrimLeft(const T* what)
	{
		if (0 == m_size)
			return;

		const auto length = Traits::GetLength(m_buffer);
		if (0 == length)
			return;

		if (nullptr == what)
			return;

		size_t i = 0;
		while (i < length && Traits::OneOf(m_buffer[i], what))
			i++;

		if (0 == i)
			return;

		StringT other(m_buffer + i, length - i, m_allocator);
		swap(*this, other);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::TrimRight(const T* what)
	{
		if (0 == m_size)
			return;

		const auto length = Traits::GetLength(m_buffer);
		if (0 == length)
			return;

		if (nullptr == what)
			return;

		auto ptr = m_buffer + length - 1;
		while (Traits::OneOf(*ptr, what) && ptr >= m_buffer)
			ptr--;

		*++ptr = 0;
	}

	template <typename T, typename Traits>
	size_t StringT<T, Traits>::GetLength(const T* string)
	{
		return Traits::GetLength(string);
	}

	template <typename T, typename Traits>
	template <typename... Ts>
	StringT<T, Traits> StringT<T, Traits>::Format(const T* format, const Ts&... ts)
	{
#ifdef _DEBUG
		Details::FormatCheck(format, Details::NormalizeArg(ts)...);
#endif
		const auto length = Traits::FormatCount(format, Details::NormalizeArg(ts)...);
		if (0 == length)
			return StringT();

		StringT result(length);
		if (nullptr != result.m_buffer)
			Traits::Format(result.m_buffer, result.m_size, format, Details::NormalizeArg(ts)...);
		return result;
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::CopyFrom(const StringT& other)
	{
		Base::CopyFrom(other);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::MoveFrom(StringT& other)
	{
		Base::MoveFrom(other);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::DoReserve(size_t length)
	{
		const auto size = (length + 1) * sizeof(T);
		DoAllocate(size);
		memset(m_buffer, 0, m_size);
	}

	template <typename T, typename Traits>
	void StringT<T, Traits>::DoReplace(size_t from, size_t whatLength, const T* with, size_t withLength)
	{
		const auto diff = static_cast<ptrdiff_t>(withLength) - static_cast<ptrdiff_t>(whatLength);
		const auto length = Traits::GetLength(m_buffer);
		const auto required = length + diff;
		const auto capacity = m_size / sizeof(T);
		if (required > capacity - 1) // Resulting string is longer than buffer
		{
			const auto source = from + whatLength;
			const auto destin = from + withLength;
			StringT<T, Traits> other(required, m_allocator);
			Traits::Copy(other.m_buffer, other.m_size / sizeof(T), m_buffer, from);
			Traits::Copy(other.m_buffer + from, other.m_size / sizeof(T) - from, with, withLength);
			Traits::Copy(other.m_buffer + destin, other.m_size / sizeof(T) - destin, m_buffer + source, m_size / sizeof(T) - source);
			swap(*this, other);
		}
		else if (diff == 0) // Resulting string has the same size
		{
			memcpy_s(m_buffer + from, m_size - from * sizeof(T), with, withLength * sizeof(T));
		}
		else // Result string is shorter than buffer
		{
			const auto source = from + whatLength;
			const auto destin = from + withLength;
			const auto length = Traits::GetLength(m_buffer + source);
			memmove_s(m_buffer + destin, m_size - destin * sizeof(T), m_buffer + source, (length + 1) * sizeof(T));
			memcpy_s(m_buffer + from, m_size - from * sizeof(T), with, withLength * sizeof(T));
		}
	}

	template <typename T, typename Traits>
	bool operator==(const T* left, const StringT<T, Traits>& right)
	{
		return right.IsEqual(left);
	}

	template <typename T, typename Traits>
	bool operator!=(const T* left, const StringT<T, Traits>& right)
	{
		return !right.IsEqual(left);
	}

	typedef StringT<char> Utf8;
	typedef StringT<wchar_t> Utf16;

	//
	// By default use UTF-16 since it's used by Windows internally
	//

	typedef Utf16 String;
}

namespace Neat { namespace Details
{
	template <typename T>
	typename std::enable_if<std::is_pointer<T>::value, T>::type NormalizeArg(T arg)
	{
		return arg;
	}

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type NormalizeArg(T arg)
	{
		return arg;
	}

	template <typename T>
	typename std::enable_if<std::is_floating_point<T>::value, double>::type NormalizeArg(T arg)
	{
		return arg;
	}

	template <typename T>
	const T* NormalizeArg(const std::basic_string<T>& arg)
	{
		return arg.c_str();
	}

	template <typename T, typename Traits>
	const T* NormalizeArg(const Neat::StringT<T, Traits>& arg)
	{
		return arg.operator const T*();
	}

	template <typename F>
	void FormatCheck(const F* f)
	{
		for (; *f; ++f)
		{
			if (*f != '%' || *++f == '%')
				continue;
			throw std::runtime_error("Bad format");
		}
	}

	template <typename F, typename T, typename... Ts>
	void FormatCheck(const F* f, const T& t, const Ts&... ts)
	{
		for (; *f; ++f)
		{
			if (*f != '%' || *++f == '%')
				continue;
			// Skip flags directives and width specification
			while (*f >= '0' && *f <= '9')
				++f;
			// Skip size prefixes: l, ll, h, w, I, I32, I64
			if (*f == 'l')
			{
				++f;
				if (*f == 'l')
					++f;
			}
			if (*f == 'h' || *f == 'w')
				++f;
			if (*f == 'I')
			{
				++f;
				if ((*f == '3' && *(f + 1) == '2') ||
					(*f == '6' && *(f + 1) == '4'))
				{
					++f;
					++f;
				}
			}
			switch (*f)
			{
				default:
					throw std::runtime_error("Invalid format char");
				case 'd':
				case 'i':
				case 'o':
				case 'u':
				case 'x':
				case 'X':
					if (std::is_integral<T>::value)
						break;
					throw std::runtime_error("Integer value expected");
				case 'e':
				case 'E':
				case 'f':
				case 'g':
				case 'G':
				case 'a':
				case 'A':
					if (std::is_floating_point<T>::value)
						break;
					throw std::runtime_error("Floating point value expected");
				case 'p':
					if (std::is_pointer<T>::value)
						break;
					throw std::runtime_error("Pointer expected");
				case 's':
					if (std::is_same<F, char>::value)
					{
						if (std::is_same<typename std::decay<T>::type, char*>::value ||
							std::is_same<typename std::decay<T>::type, const char*>::value)
							break;
						throw std::runtime_error("Utf8 string expected");
					}
					else if (std::is_same<F, wchar_t>::value)
					{
						if (std::is_same<typename std::decay<T>::type, wchar_t*>::value ||
							std::is_same<typename std::decay<T>::type, const wchar_t*>::value)
							break;
						throw std::runtime_error("Utf16 string expected");
					}
					else
					{
						throw std::runtime_error("Unsupported type");
					}
				case 'S':
					if (std::is_same<F, wchar_t>::value)
					{
						if (std::is_same<typename std::decay<T>::type, char*>::value ||
							std::is_same<typename std::decay<T>::type, const char*>::value)
							break;
						throw std::runtime_error("Utf8 string expected");
					}
					else if (std::is_same<F, char>::value)
					{
						if (std::is_same<typename std::decay<T>::type, wchar_t*>::value ||
							std::is_same<typename std::decay<T>::type, const wchar_t*>::value)
							break;
						throw std::runtime_error("Utf16 string expected");
					}
					else
					{
						throw std::runtime_error("Unsupported type");
					}
			}
			return FormatCheck(f, ts...);
		}
		throw std::runtime_error("Too few format specifiers");
	}

} }