#include "Neat\CmdLine.h"

namespace Neat
{
	using namespace std;

	CmdLineOption::CmdLineOption()
	{
	}

	CmdLineOption::CmdLineOption(const char* name) :
		m_name(name)
	{
	}

	CmdLine::CmdLine()
	{
	}

	void CmdLine::Parse(int argc, const char* argv[])
	{
		if (argc >= 1)
			m_app = argv[0];

		bool ignoreRest = false;
		string_view lastCommand;
		for (auto i = 1; i < argc; i++)
		{
			string_view arg = argv[i];
			if (ignoreRest)
			{
				m_params.push_back(arg);
				continue;
			}
			auto pair = ParseCommand(arg);
			if (pair.first.empty())
			{
				if (pair.second.empty())
				{
					ignoreRest = true;
					continue;
				}
				if (lastCommand.empty())
				{
					m_params.push_back(arg);
					continue;
				}
				m_options[lastCommand].m_params.push_back(pair.second);
				continue;
			}
			auto& option = m_options[pair.first];
			option.m_name = pair.first;
			if (pair.second.empty())
			{
				lastCommand = pair.first;
				continue;
			}
			lastCommand = string_view();
			option.m_params.push_back(pair.second);
		}
	}

	pair<string_view, string_view> CmdLine::ParseCommand(
		const string_view& command)
	{
		if (0 == command.compare(0, 2, "--"))
		{
			const auto pos = command.find('=');
			if (string_view::npos == pos)
				return make_pair(command.substr(2), string_view());
			return make_pair(command.substr(2, pos - 2), command.substr(pos + 1));
		}
		if (0 == command.compare(0, 1, "-"))
		{
			const auto pos = command.find('=');
			if (string_view::npos == pos)
				return make_pair(command.substr(1), string_view());
			return make_pair(command.substr(1, pos - 1), command.substr(pos + 1));
		}
		return make_pair(string_view(), command);
	}
}