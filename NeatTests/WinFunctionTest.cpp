#include "CppUnitTest.h"

#include <Neat\Win\Function.h>

#include <Windows.h>

typedef struct _SIGNER_CONTEXT
{
	DWORD cbSize;
	DWORD cbBlob;
	BYTE *pbBlob;
} SIGNER_CONTEXT, *PSIGNER_CONTEXT;

typedef HRESULT(WINAPI* SignerFreeSignerContext)(
	__in  SIGNER_CONTEXT *pSignerContext
);

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Neat::Win
{
	TEST_CLASS(WinFunctionTest)
	{
	public:
		TEST_METHOD(FunctionWrapper)
		{
			{
				const auto moduleName = "Mssign32.dll";
				const auto functionName = "SignerFreeSignerContext";

				Assert::IsNull(::GetModuleHandleA(moduleName));
				{
					const auto signerFreeSignerContext = Function<SignerFreeSignerContext>(moduleName, functionName);
					Assert::IsTrue(signerFreeSignerContext);
					Assert::IsNotNull(::GetModuleHandleA(moduleName));

					PSIGNER_CONTEXT context = nullptr;
					auto hr = signerFreeSignerContext(context);
					Assert::IsTrue(SUCCEEDED(hr));
				}
				Assert::IsNull(::GetModuleHandleA(moduleName));
			}
			{
				const auto moduleName = "BadFileName";
				const auto functionName = "SignerFreeSignerContext";

				const auto signerFreeSignerContext = Function<SignerFreeSignerContext>(moduleName, functionName);
				Assert::IsFalse(signerFreeSignerContext);
			}
			{
				const auto moduleName = "Mssign32.dll";
				const auto functionName = "BadFunctionName";

				const auto signerFreeSignerContext = Function<SignerFreeSignerContext>(moduleName, functionName);
				Assert::IsFalse(signerFreeSignerContext);
			}
		}
	};
}