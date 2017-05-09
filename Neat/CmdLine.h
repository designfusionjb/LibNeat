#pragma once

#include <map>
#include <string_view>
#include <vector>

namespace Neat
{
	class CmdLineOption
	{
	public:
		CmdLineOption();
		CmdLineOption(const char* name);

	private:
		std::string_view m_name;
		std::vector<std::string_view> m_params;

		friend class CmdLine;
		friend class CmdLineTest;
	};

	class CmdLine
	{
	public:
		CmdLine();

		void Parse(int argc, const char* argv[]);

	protected:
		static std::pair<std::string_view, std::string_view> ParseCommand(
			const std::string_view& command);

	private:
		std::map<std::string_view, CmdLineOption> m_options;
		std::vector<std::string_view> m_params;
		std::string_view m_app;

		friend class CmdLineTest;
	};
}
