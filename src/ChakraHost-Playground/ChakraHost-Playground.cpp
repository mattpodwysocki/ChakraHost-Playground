// ChakraHost-Playground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChakraHost.h"

int RunScript()
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

	// Console logs empty object
	JsValueRef result;
	const wchar_t* szScript = L"(() => { return function(x, y) { console.log(arguments); return x + y; }; })()";
	status = host.RunScript(szScript, L"", &result);

	JsPropertyIdRef applyId;
	status = JsGetPropertyIdFromName(L"apply", &applyId);

	JsValueRef applyObj;
	status = JsGetProperty(result, applyId, &applyObj);

	JsValueRef args;
	status = host.RunScript(L"(() => [1,2])()", L"", &args);

	JsValueRef argsArray;
	status = JsCreateArray(2, &argsArray);

	JsPropertyIdRef arg0Id, arg1Id;
	JsValueRef arg0, arg1;
	JsValueRef arg0Val, arg1Val;
	JsGetPropertyIdFromName(L"0", &arg0Id);
	JsGetPropertyIdFromName(L"1", &arg1Id);
	JsGetProperty(argsArray, arg0Id, &arg0);
	JsGetProperty(argsArray, arg1Id, &arg1);
	JsIntToNumber(42, &arg0Val);
	JsIntToNumber(56, &arg1Val);

	JsSetIndexedProperty(argsArray, arg0, arg0Val);
	JsSetIndexedProperty(argsArray, arg1, arg1Val);

	JsValueRef argObj[2] = { result , argsArray };
	JsValueRef returnObj;
	status = JsCallFunction(applyObj, argObj, 2, &returnObj);

	JsValueRef stringObj;
	status = JsConvertValueToString(returnObj, &stringObj);

	const wchar_t* szBuf;
	size_t bufLen;
	status = JsStringToPointer(stringObj, &szBuf, &bufLen);

	// Prints out NaN
	wprintf(L"Result: %s", szBuf);
cleanup:

	status = host.Destroy();
	if (status != JsNoError)
	{
		wprintf(L"Error in destroying runtime");
	}

	return status;
}

