#include "CppUnitTest.h"

#include <Neat\CmdLine.h>
#include <Neat\Types.h>

#include <iterator>
#include <string_view>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace std
{
	inline wstring ToString(const string_view& value)
	{
		return Microsoft::VisualStudio::CppUnitTestFramework::ToString(string(value.data(), value.size()));
	}
}

namespace Neat
{
	TEST_CLASS(CmdLineTest)
	{
	public:
		TEST_METHOD(CmdLineParseCommand)
		{
			{
				const auto pair = CmdLine::ParseCommand("--foo");
				Assert::AreEqual(std::string_view("foo"), pair.first);
				Assert::AreEqual(std::string_view(), pair.second);
			}
			{
				const auto pair = CmdLine::ParseCommand("--foo=bar");
				Assert::AreEqual(std::string_view("foo"), pair.first);
				Assert::AreEqual(std::string_view("bar"), pair.second);
			}
			{
				const auto pair = CmdLine::ParseCommand("-f");
				Assert::AreEqual(std::string_view("f"), pair.first);
				Assert::AreEqual(std::string_view(), pair.second);
			}
			{
				const auto pair = CmdLine::ParseCommand("-f=bar");
				Assert::AreEqual(std::string_view("f"), pair.first);
				Assert::AreEqual(std::string_view("bar"), pair.second);
			}
			{
				const auto pair = CmdLine::ParseCommand("bar");
				Assert::AreEqual(std::string_view(), pair.first);
				Assert::AreEqual(std::string_view("bar"), pair.second);
			}
			{
				const auto pair = CmdLine::ParseCommand("--");
				Assert::AreEqual(std::string_view(), pair.first);
				Assert::AreEqual(std::string_view(), pair.second);
			}
			{
				const auto pair = CmdLine::ParseCommand("");
				Assert::AreEqual(std::string_view(), pair.first);
				Assert::AreEqual(std::string_view(), pair.second);
			}
		}

		TEST_METHOD(CmdLineParse)
		{
			{
				const char* argv[] =
				{
					"app.exe",
					"-1",
					"1-value",
					"-2=2-value",
					"value1",
					"--opt1",
					"opt1-value1",
					"opt1-value2",
					"--opt2=opt2-value",
					"-k",
					"--key",
					"--",
					"--not-a-command",
					"value2"
				};
				const int argc = std::size(argv);

				CmdLine cmdLine;
				cmdLine.Parse(argc, argv);

				Assert::AreEqual(6_sz, cmdLine.m_options.size());
				{
					const auto& option = cmdLine.m_options["1"];
					Assert::AreEqual(std::string_view("1"), option.m_name);
					Assert::AreEqual(1_sz, option.m_params.size());
					Assert::AreEqual(std::string_view("1-value"), option.m_params[0]);
				}
				{
					const auto& option = cmdLine.m_options["2"];
					Assert::AreEqual(std::string_view("2"), option.m_name);
					Assert::AreEqual(1_sz, option.m_params.size());
					Assert::AreEqual(std::string_view("2-value"), option.m_params[0]);
				}
				{
					const auto& option = cmdLine.m_options["opt1"];
					Assert::AreEqual(std::string_view("opt1"), option.m_name);
					Assert::AreEqual(2_sz, option.m_params.size());
					Assert::AreEqual(std::string_view("opt1-value1"), option.m_params[0]);
					Assert::AreEqual(std::string_view("opt1-value2"), option.m_params[1]);
				}
				{
					const auto& option = cmdLine.m_options["opt2"];
					Assert::AreEqual(std::string_view("opt2"), option.m_name);
					Assert::AreEqual(1_sz, option.m_params.size());
					Assert::AreEqual(std::string_view("opt2-value"), option.m_params[0]);
				}
				{
					const auto& option = cmdLine.m_options["k"];
					Assert::AreEqual(std::string_view("k"), option.m_name);
					Assert::AreEqual(0_sz, option.m_params.size());
				}
				{
					const auto& option = cmdLine.m_options["key"];
					Assert::AreEqual(std::string_view("key"), option.m_name);
					Assert::AreEqual(0_sz, option.m_params.size());
				}
				
				Assert::AreEqual(3_sz, cmdLine.m_params.size());
				Assert::AreEqual(std::string_view("value1"), cmdLine.m_params[0]);
				Assert::AreEqual(std::string_view("--not-a-command"), cmdLine.m_params[1]);
				Assert::AreEqual(std::string_view("value2"), cmdLine.m_params[2]);

				Assert::AreEqual(std::string_view("app.exe"), cmdLine.m_app);
			}
		}
	};
}