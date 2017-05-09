#pragma once
#include "Neat\Win\Handle.h"

#include <Windows.h>

namespace Neat::Win
{
	template <typename Prototype>
	class FunctionWrapper
	{
	public:
		FunctionWrapper(
			const char* moduleName,
			const char* functionName);

		operator bool() const;

		template <typename... ArgTypes>
		auto operator()(ArgTypes&&... args) const;

	private:
		Prototype m_function;
		ModuleHandle m_module;
	};

	template <typename Prototype>
	inline FunctionWrapper<Prototype>::FunctionWrapper(
		const char* moduleName,
		const char* functionName) :
		m_function(nullptr)
	{
		m_module = ::LoadLibraryA(moduleName);
		m_function = (Prototype)::GetProcAddress(m_module, functionName);
	}

	template <typename Prototype>
	inline FunctionWrapper<Prototype>::operator bool() const
	{
		return m_function != nullptr;
	}

	template <typename Prototype>
	template <typename... ArgTypes>
	inline auto FunctionWrapper<Prototype>::operator()(ArgTypes&&... args) const
	{
		return m_function(std::forward<ArgTypes>(args)...);
	}

	template <typename Prototype>
	FunctionWrapper<Prototype> Function(const char* moduleName, const char* functionName)
	{
		return FunctionWrapper<Prototype>(moduleName, functionName);
	}

	template <typename Prototype>
	Prototype Function(const HMODULE moduleHandle, const char* functionName)
	{
		return (Prototype)::GetProcAddress(moduleHandle, functionName);
	}
}