#pragma once
#include "Neat\Types.h"
#include "Neat\Utf.h"

#include <utility>

namespace Neat::Win
{
	template <typename T>
	class PathT : public StringT<T>
	{
		typedef StringT<T> Base;

	public:
		PathT(const T* path = nullptr);
		PathT(StringT<T>&& path);

		PathT(PathT&& other);
		PathT(const PathT& other);

		PathT& operator=(PathT&& other);
		PathT& operator=(const PathT& other);

		PathT& operator+=(const PathT& other);
		PathT operator+(const PathT& other) const;

		void Append(const T* path, size_t length = Base::End);
		void Append(const T t) = delete;

		void ReplaceName(const StringT<T>& other);
		void ReplaceFullName(const StringT<T>& other);
		void ReplaceExtension(const StringT<T>& other);
		void ReplaceDirectory(const StringT<T>& other);

		StringT<T> GetName() const;
		StringT<T> GetNameWithoutExtension() const;
		StringT<T> GetFolder() const;

		static PathT GetCurrent();
		static PathT UrlToWin32(const T* url);
		static PathT GetFileName(StringT<T> cmdLine);
		static PathT NtToWin32(const T* ntPath, bool kernelRules);
		static PathT SearchFullPath(const T* fileName);
		static PathT GetFullPath(const T* path);
		static std::pair<StringT<T>, StringT<T>> SplitCommandLine(const T* commandLine);

	protected:
		void MoveFrom(PathT& other);
		void CopyFrom(const PathT& other);
	};

	template <typename T>
	PathT<T>::PathT(const T* path)
		: Base(path)
	{
	}

	template <typename T>
	PathT<T>::PathT(StringT<T>&& path) :
		Base(std::move(path))
	{
	}

	template <typename T>
	PathT<T>::PathT(PathT&& other)
	{
		MoveFrom(other);
	}

	template <typename T>
	PathT<T>::PathT(const PathT& other)
	{
		CopyFrom(other);
	}

	template <typename T>
	PathT<T>& PathT<T>::operator=(PathT&& other)
	{
		MoveFrom(other);
		return *this;
	}

	template <typename T>
	PathT<T>& PathT<T>::operator=(const PathT& other)
	{
		CopyFrom(other);
		return *this;
	}

	template <typename T>
	PathT<T>& PathT<T>::operator+=(const PathT& other)
	{
		Append(other);
		return *this;
	}

	template <typename T>
	PathT<T> PathT<T>::operator+(const PathT& other) const
	{
		PathT path(*this);
		path.Append(other);
		return path;
	}

	template <typename T>
	void PathT<T>::Append(const T* path, size_t length)
	{
		if (Base::End == length)
			length = Base::GetLength(path);

		if (0 == length)
			return;

		if (IsEmpty())
		{
			Base::Append(path, length);
			return;
		}

		if (!EndsWith('\\'))
		{
			if (path[0_sz] != '\\')
				Base::Append('\\');

			Base::Append(path, length);
		}
		else
		{
			if (path[0_sz] == '\\')
				Base::Append(path + 1, length - 1);
			else
				Base::Append(path, length);
		}
	}

	template <typename T>
	void PathT<T>::ReplaceName(const StringT<T>& name)
	{
		auto slash = FindLast('\\');
		auto dot = FindLast('.');

		if (Base::End != slash)
		{
			if (Base::End != dot)
				Replace(++slash, dot, name);
			else
				Replace(++slash, name);
		}
		else
		{
			if (Base::End != dot)
				Replace(0, dot, name);
			else
				static_cast<StringT<T>&>(*this) = name;
		}
	}

	template <typename T>
	void PathT<T>::ReplaceFullName(const StringT<T>& fullName)
	{
		auto slash = FindLast('\\');
		if (Base::End != slash)
			Replace(++slash, fullName);
		else
			static_cast<StringT<T>&>(*this) = fullName;
	}

	template <typename T>
	void PathT<T>::ReplaceExtension(const StringT<T>& ext)
	{
		auto dot = FindLast('.');
		if (Base::End != dot)
		{
			Replace(++dot, ext);
		}
		else
		{
			static_cast<StringT<T>&>(*this) += '.';
			static_cast<StringT<T>&>(*this) += ext;
		}
	}

	template <typename T>
	void PathT<T>::ReplaceDirectory(const StringT<T>& dir)
	{
		auto slash = FindLast('\\');
		if (Base::End != slash)
		{
			Replace(0, slash, dir);
		}
		else
		{
			auto temp = *this;
			static_cast<StringT<T>&>(*this) = dir;
			Append(temp);
		}
	}

	template <typename T>
	StringT<T> PathT<T>::GetName() const
	{
		auto slash = FindLast('\\');
		if (Base::End != slash)
			return Substring(++slash);
		return *this;
	}

	template <typename T>
	StringT<T> PathT<T>::GetNameWithoutExtension() const
	{
		auto dot = FindLast('.');
		auto slash = FindLast('\\');
		if (Base::End != slash)
		{
			slash++;
			if (Base::End != dot && dot >= slash)
			{
				auto size = dot - slash;
				return Substring(slash, size);
			}
			else
			{
				return Substring(slash);
			}
		}
		else if (Base::End != dot)
		{
			return Substring(0, dot);
		}
		return *this;
	}

	template <typename T>
	StringT<T> PathT<T>::GetFolder() const
	{
		auto slash = FindLast('\\');
		if (0 == slash)
			return *this;

		if (Base::End != slash)
		{
			if (m_buffer[slash - 1] == ':')
				return Substring(0, slash + 1);
			return Substring(0, slash);
		}
		return StringT<T>(0, nullptr);
	}

	template <typename T>
	void PathT<T>::MoveFrom(PathT& other)
	{
		if (this != &other)
		{
			Base::MoveFrom(other);
		}
	}

	template <typename T>
	void PathT<T>::CopyFrom(const PathT& other)
	{
		if (this != &other)
		{
			Base::CopyFrom(other);
		}
	}

	typedef PathT<char> Path8;
	typedef PathT<wchar_t> Path16;

	// Default for Windows platform
	using Path = Path16;
}