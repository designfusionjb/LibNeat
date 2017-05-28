#include "stdafx.h"
#include <Neat\Win\Path.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat::Win
{
	TEST_CLASS(PathTest)
	{
	public:
		TEST_METHOD(Path_Append)
		{
			[]() // Simplest case
			{
				Path path;
				Assert::AreEqual(nullptr, static_cast<const String&>(path));

				path.Append(L"Dir");
				Assert::AreEqual(L"Dir", static_cast<const String&>(path));
			
				path.Append(L"SubDir");
				Assert::AreEqual(L"Dir\\SubDir", static_cast<const String&>(path));
			
				path.Append(L"");
				Assert::AreEqual(L"Dir\\SubDir", static_cast<const String&>(path));
			}();

			[]() // More backslashes
			{
				Path path(L"Dir\\");
				Assert::AreEqual(L"Dir\\", static_cast<const String&>(path));

				path.Append(L"\\SubDir");
				Assert::AreEqual(L"Dir\\SubDir", static_cast<const String&>(path));
			
				path.Append(L"\\FileName.Ext");
				Assert::AreEqual(L"Dir\\SubDir\\FileName.Ext", static_cast<const String&>(path));
			}();

			[]() // Several paths
			{
				Path path(L"RootDir\\");

				path.Append(Path(L"Some\\Nested\\Dirs\\"));
				Assert::AreEqual(L"RootDir\\Some\\Nested\\Dirs\\", static_cast<const String&>(path));

				path.Append(Path(L"FileName.Ext"));
				Assert::AreEqual(L"RootDir\\Some\\Nested\\Dirs\\FileName.Ext", static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(Path_ReplaceName)
		{
			[]() // Simplest case
			{
				Path path(L"C:\\Dir\\SubDir\\FileName.Ext");

				path.ReplaceName(L"AnotherName");
				Assert::AreEqual(L"AnotherName.Ext", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\AnotherName.Ext", static_cast<const String&>(path));

				path.ReplaceName(L"Small");
				Assert::AreEqual(L"Small.Ext", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\Small.Ext", static_cast<const String&>(path));
			}();

			[]() // No extension
			{
				Path path(L"C:\\Dir\\SubDir\\FileName");

				path.ReplaceName(L"AnotherName");
				Assert::AreEqual(L"AnotherName", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\AnotherName", static_cast<const String&>(path));
			}();

			[]() // No backslashes
			{
				Path path(L"FileName.Ext");

				path.ReplaceName(L"AnotherName");
				Assert::AreEqual(L"AnotherName.Ext", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"AnotherName.Ext", static_cast<const String&>(path));

				path.ReplaceName(L"Small");
				Assert::AreEqual(L"Small.Ext", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"Small.Ext", static_cast<const String&>(path));
			}();

			[]() // No extension and no backslashes
			{
				Path path(L"FileName");

				path.ReplaceName(L"AnotherName");
				Assert::AreEqual(L"AnotherName", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"AnotherName", static_cast<const String&>(path));

				path.ReplaceName(L"Small");
				Assert::AreEqual(L"Small", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"Small", static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(Path_ReplaceFullName)
		{
			[]() // Full path
			{
				Path path(L"C:\\Dir\\SubDir\\Name.Ext");

				path.ReplaceFullName(L"FullName.WithExt");
				Assert::AreEqual(L"FullName.WithExt", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\FullName.WithExt", static_cast<const String&>(path));

				path.ReplaceFullName(L"FullNameWithOutExt");
				Assert::AreEqual(L"FullNameWithOutExt", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\FullNameWithOutExt", static_cast<const String&>(path));
			}();

			[]() // No backslashes
			{
				Path path(L"Name.Ext");

				path.ReplaceFullName(L"FullNameWithOutExt");
				Assert::AreEqual(L"FullNameWithOutExt", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"FullNameWithOutExt", static_cast<const String&>(path));

				path.ReplaceFullName(L"FullName.WithExt");
				Assert::AreEqual(L"FullName.WithExt", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"FullName.WithExt", static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(Path_ReplaceExtension)
		{
			[]() // Simplest case
			{
				Path path(L"C:\\Dir\\SubDir\\FileName.Ext");

				path.ReplaceExtension(L"AnotherExt");
				Assert::AreEqual(L"FileName.AnotherExt", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\FileName.AnotherExt", static_cast<const String&>(path));

				path.ReplaceExtension(L"SomeExt");
				Assert::AreEqual(L"FileName.SomeExt", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\FileName.SomeExt", static_cast<const String&>(path));
			}();

			[]() // Without extension
			{
				Path path(L"C:\\Dir\\SubDir\\FileName");

				path.ReplaceExtension(L"Ext");
				Assert::AreEqual(L"FileName.Ext", static_cast<const String&>(path.GetName()));
				Assert::AreEqual(L"C:\\Dir\\SubDir\\FileName.Ext", static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(Path_ReplaceDirectory)
		{
			[]() // Simplest case
			{
				Path path(L"D:\\Dir\\SubDir\\FileName.Ext");

				path.ReplaceDirectory(L"C:\\AnotherDir\\OneMore");
				Assert::AreEqual(L"C:\\AnotherDir\\OneMore\\FileName.Ext", static_cast<const String&>(path));
			}();

			[]() // Without backslashes
			{
				Path path(L"FileName");

				path.ReplaceDirectory(L"Dir\\SubDir");
				Assert::AreEqual(L"Dir\\SubDir\\FileName", static_cast<const String&>(path));
			}();
		}
		
		TEST_METHOD(Path_GetFileName)
		{
			auto check = [](const wchar_t* input, const wchar_t* expectedOutput)
			{
				auto path = Path::GetFileName(input);
				Assert::AreEqual(expectedOutput, static_cast<const String&>(path));
			};

			check(
				LR"(C:\Windows\System32\StikyNot.exe)",
				LR"(C:\Windows\System32\StikyNot.exe)");

			check(
				LR"("C:\Program Files\Microsoft Device Center\itype.exe")",
				LR"(C:\Program Files\Microsoft Device Center\itype.exe)");

			check(
				LR"("C:\Program Files\Microsoft Office\Office14\BCSSync.exe" /DelayServices)",
				LR"(C:\Program Files\Microsoft Office\Office14\BCSSync.exe)");

			check(
				LR"(C:\Windows\system32\rundll32.exe C:\Windows\System32\LogiLDA.dll,LogiFetch)",
				LR"(C:\Windows\System32\LogiLDA.dll)");
			
			check(
				LR"(C:\Windows\system32\Rundll32.exe C:\Windows\System32\LogiLDA.dll,LogiFetch)",
				LR"(C:\Windows\System32\LogiLDA.dll)");
			
			check(
				LR"(C:\Windows\system32\RunDll32.exe "D:\Foo Bar\bad.dll",DllRun)",
				LR"(D:\Foo Bar\bad.dll)");
			
			check(
				LR"("C:\Windows\system32\RunDll32.exe" "D:\Foo Bar\bad.dll",DllRun)",
				LR"(D:\Foo Bar\bad.dll)");

			check(
				LR"(C:\Program Files\NVIDIA Corporation\Installer2\installer.{DC9636E6-1588-49A3-9AA2-9BC0C34AA837}\NVI2.dll,0)",
				LR"(C:\Program Files\NVIDIA Corporation\Installer2\installer.{DC9636E6-1588-49A3-9AA2-9BC0C34AA837}\NVI2.dll)");

			check(
				LR"(C:\Program Files\Common Files\Microsoft Shared\OFFICE14\Office Setup Controller\OSETUP.DLL,1)",
				LR"(C:\Program Files\Common Files\Microsoft Shared\OFFICE14\Office Setup Controller\OSETUP.DLL)");

			check(
				LR"(C:\Windows\Microsoft.NET\Framework64\v4.0.30319\SetupCache\\DisplayIcon.ico)",
				LR"(C:\Windows\Microsoft.NET\Framework64\v4.0.30319\SetupCache\DisplayIcon.ico)");

			check(
				LR"(C:\Program Files\Windows Defender\MSASCui.exe -hide)",
				LR"(C:\Program Files\Windows Defender\MSASCui.exe)");
			
			check(
				LR"(C:\Program Files\Windows Defender\MSASCui.EXE -hide)",
				LR"(C:\Program Files\Windows Defender\MSASCui.EXE)");
			
			check(
				LR"(\??\C:\WINDOWS\system32\winlogon.exe)",
				LR"(C:\WINDOWS\system32\winlogon.exe)");
			
		}
		
		TEST_METHOD(Path_NtToWin32)
		{
			// Check kernel convertion rules
			{
				auto check = [](const wchar_t* input, const wchar_t* expectedOutput)
				{
					auto path = Path::NtToWin32(input, true);
					Assert::AreEqual(expectedOutput, static_cast<const String&>(path), true);
				};

				check(
					L"\\SystemRoot\\System32\\ntoskrnl.exe",
					L"C:\\Windows\\System32\\ntoskrnl.exe");

				check(
					L"\\SystemRoot\\System32\\Drivers\\Ntfs.sys",
					L"C:\\Windows\\System32\\Drivers\\Ntfs.sys");

				check(
					L"\\??\\C:\\Windows\\system32\\drivers\\hcmon.sys",
					L"C:\\Windows\\system32\\drivers\\hcmon.sys");

				check(
					L"\\WINDOWS\\system32\\ntkrnlpa.exe",
					L"C:\\WINDOWS\\system32\\ntkrnlpa.exe");

				check(
					L"pci.sys",
					L"C:\\Windows\\System32\\Drivers\\pci.sys");
			}

			// Check non kernel convertion rules
			{
				auto check = [](const wchar_t* input, const wchar_t* expectedOutput)
				{
					auto path = Path::NtToWin32(input, false);
					Assert::AreEqual(expectedOutput, static_cast<const String&>(path), true);
				};

				check(
					L"\\SystemRoot\\System32\\ntoskrnl.exe",
					L"C:\\Windows\\System32\\ntoskrnl.exe");

				check(
					L"\\SystemRoot\\System32\\Drivers\\Ntfs.sys",
					L"C:\\Windows\\System32\\Drivers\\Ntfs.sys");

				check(
					L"\\??\\C:\\WINDOWS\\system32\\winlogon.exe",
					L"C:\\WINDOWS\\system32\\winlogon.exe");

				check(
					L"\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe",
					L"\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe");

				check(
					L"services.exe",
					L"services.exe");
			}
		}
		
		TEST_METHOD(Path_GetName)
		{
			auto check = [](const wchar_t* input, const wchar_t* expectedOutput)
			{
				Path path(input);
				Assert::AreEqual(expectedOutput, path.GetName());
			};

			check(
				L"C:\\Windows\\System32\\StikyNot.exe",
				L"StikyNot.exe");

			check(
				L"C:\\Windows\\System32\\",
				L"");

			check(
				L"C:\\Windows",
				L"Windows");

			check(
				L"services.exe",
				L"services.exe");

			check(
				L"\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe",
				L"App.exe");

			check(
				L"\\SystemRoot\\System32\\ntoskrnl.exe",
				L"ntoskrnl.exe");

			check(
				L"\\SystemRoot",
				L"SystemRoot");
		}
		
		TEST_METHOD(Path_GetNameWithoutExtension)
		{
			auto check = [](const wchar_t* input, const wchar_t* expectedOutput)
			{
				Path path(input);
				Assert::AreEqual(expectedOutput, path.GetNameWithoutExtension());
			};

			check(
				L"C:\\Windows\\System32\\StikyNot.exe",
				L"StikyNot");

			check(
				L"C:\\Some.Folder\\SomeFile",
				L"SomeFile");

			check(
				L"C:\\SomeFolder\\.SomeFile",
				L"");

			check(
				L"C:\\Windows\\System32\\",
				L"");

			check(
				L"C:\\Windows",
				L"Windows");

			check(
				L"services.exe",
				L"services");

			check(
				L"\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe",
				L"App");

			check(
				L"\\SystemRoot\\System32\\ntoskrnl.exe",
				L"ntoskrnl");

			check(
				L"\\SystemRoot",
				L"SystemRoot");
		}
		
		TEST_METHOD(Path_GetFolder)
		{
			auto check = [](const wchar_t* input, const wchar_t* expectedOutput)
			{
				Path path(input);
				Assert::AreEqual(expectedOutput, path.GetFolder());
			};

			check(
				L"C:\\Windows\\System32\\StikyNot.exe",
				L"C:\\Windows\\System32");

			check(
				L"C:\\Windows\\System32\\",
				L"C:\\Windows\\System32");

			check(
				L"C:\\Windows",
				L"C:\\");

			check(
				L"services.exe",
				L"");

			check(
				L"\\\\vmware-host\\Shared Folders\\Shared VMs Folder\\App.exe",
				L"\\\\vmware-host\\Shared Folders\\Shared VMs Folder");

			check(
				L"\\SystemRoot\\System32\\ntoskrnl.exe",
				L"\\SystemRoot\\System32");

			check(
				L"\\SystemRoot",
				L"\\SystemRoot");
		}

		TEST_METHOD(Path_SearchFullPath)
		{
			{
				auto fileName = String(L"cmd.exe");
				auto fullPath = Path::SearchFullPath(fileName);
				
				Assert::IsFalse(fullPath.IsEmpty());
				Assert::IsFalse(fullPath == fileName);
				Assert::IsTrue(fullPath.EndsWith(fileName));
			}

			{
				auto fileName = L"h384hsld0sh3njfd0ijfdd.exe";
				auto fullPath = Path::SearchFullPath(fileName);
				
				Assert::IsTrue(fullPath == fileName);
			}
		}

		TEST_METHOD(Path_SplitCommandLine)
		{
			// Invalid command line
			{
				Assert::ExpectException<std::runtime_error>([]()
				{
					Path::SplitCommandLine(nullptr);
				});
				Assert::ExpectException<std::runtime_error>([]()
				{
					Path::SplitCommandLine(L"");
				});
			}
			// MSI
			{
				const auto pair = Path::SplitCommandLine(
					L"MsiExec.exe /X{FAAD7243-0141-3987-AA2F-E56B20F80E41}");

				Assert::AreEqual(L"MsiExec.exe", pair.first);
				Assert::AreEqual(L"/X{FAAD7243-0141-3987-AA2F-E56B20F80E41}", pair.second);
			}
			// Single path with quotes
			{
				const auto pair = Path::SplitCommandLine(
					LR"("C:\Program Files\Wireshark\uninstall.exe")");

				Assert::AreEqual(LR"(C:\Program Files\Wireshark\uninstall.exe)", pair.first);
				Assert::AreEqual(L"", pair.second);
			}
			// Single path without quotes
			{
				const auto pair = Path::SplitCommandLine(
					LR"(C:\Program Files (x86)\WinPcap\uninstall.exe)");

				Assert::AreEqual(LR"(C:\Program Files (x86)\WinPcap\uninstall.exe)", pair.first);
				Assert::AreEqual(L"", pair.second);
			}
			// Complex path 1
			{
				const auto pair = Path::SplitCommandLine(
					LR"(rundll32.exe advpack.dll,LaunchINFSectionEx "C:\Program Files\Notepad2\Notepad2.inf",DefaultUninstall,,8,N)");

				Assert::AreEqual(LR"(rundll32.exe)", pair.first);
				Assert::AreEqual(LR"(advpack.dll,LaunchINFSectionEx "C:\Program Files\Notepad2\Notepad2.inf",DefaultUninstall,,8,N)", pair.second);
			}
			// Complex path 2
			{
				const auto pair = Path::SplitCommandLine(
					LR"("C:\Program Files\Common Files\Microsoft Shared\ClickToRun\OfficeClickToRun.exe" scenario=install scenariosubtype=ARP sourcetype=None productstoremove=O365HomePremRetail.16_en-us_x-none culture=en-us)");

				Assert::AreEqual(LR"(C:\Program Files\Common Files\Microsoft Shared\ClickToRun\OfficeClickToRun.exe)", pair.first);
				Assert::AreEqual(LR"(scenario=install scenariosubtype=ARP sourcetype=None productstoremove=O365HomePremRetail.16_en-us_x-none culture=en-us)", pair.second);
			}
			// Complex path 3
			{
				const auto pair = Path::SplitCommandLine(
					LR"(%windir%\system32\sdbinst.exe -u "C:\Windows\AppPatch\Custom\Custom64\{08274920-8908-45c2-9258-8ad67ff77b09}.sdb")");

				Assert::AreEqual(LR"(%windir%\system32\sdbinst.exe)", pair.first);
				Assert::AreEqual(LR"(-u "C:\Windows\AppPatch\Custom\Custom64\{08274920-8908-45c2-9258-8ad67ff77b09}.sdb")", pair.second);
			}
		}
	};
}