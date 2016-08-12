// ChakraHost-Playground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChakraHost.h"

int main()
{
	ChakraHost host;
	JsErrorCode status = JsNoError;

	status = host.Init();
	if (status != JsNoError)
	{
		wprintf(L"Error in initializing runtime\n");
		return status;
	}

	wprintf(L"Runtime initialized\n");

	JsValueRef result;
	const wchar_t* szScript = L"(() => { console.log(42, \'foo\', 56); return \'bar\'; })();";
	status = host.RunScript(szScript, L"", &result);
	if (status != JsNoError)
	{
		wprintf(L"Failed to run script\n");
		goto cleanup;
	}

	JsValueRef resultJSString;
	if (JsConvertValueToString(result, &resultJSString) != JsNoError)
	{
		wprintf(L"Failed to convert to string");
		goto cleanup;
	}

	const wchar_t *szResult;
	size_t szResultSize;
	if (JsStringToPointer(resultJSString, &szResult, &szResultSize) != JsNoError)
	{
		wprintf(L"Failed to convert string to pointer\n");
		goto cleanup;
	}

	wprintf(L"Result: %s\n", szResult);

	system("pause");

cleanup:

	status = host.Destroy();
	if (status != JsNoError)
	{
		wprintf(L"Error in destroying runtime");
	}

    return status;
}

