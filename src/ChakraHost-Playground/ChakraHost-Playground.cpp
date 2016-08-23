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

int SerializeScript()
{
    ChakraHost host;
    JsErrorCode status = JsNoError;

    status = host.Init();
    if (status != JsNoError)
    {
        wprintf(L"Error in initializing runtime\n");
        goto cleanup;
    }

    const wchar_t* szFile = L"foo.js";
    const wchar_t* szSerializedFile = L"foo.bin";
    const wchar_t* szScript = L"(() => { return function add(x, y) { return x + y; }; })()";

    FILE* file = NULL;
    _wfopen_s(&file, szFile, L"wb");
    fwrite(szScript, sizeof(wchar_t), wcslen(szScript), file);
    fclose(file);

    JsValueRef addFunction;
    IfFailCleanup(host.SerializeScript(szScript, szSerializedFile));
    IfFailCleanup(host.RunSerializedScriptFromFile(szSerializedFile, szFile, L"", &addFunction));

    JsValueRef arg1, arg2, result;
    IfFailCleanup(JsIntToNumber(12, &arg1));
    IfFailCleanup(JsIntToNumber(34, &arg2));
    JsValueRef args[] = { host.globalObject, arg1, arg2 };
    IfFailCleanup(JsCallFunction(addFunction, args, 3, &result));

    int resultInt;
    IfFailCleanup(JsNumberToInt(result, &resultInt));
    
    wprintf(L"Result: %d", resultInt);
cleanup:
    status = host.Destroy();
    if (status != JsNoError)
    {
        wprintf(L"Error in destroying runtime");
    }

    return status;
}

int CallApply()
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
    const wchar_t* szScript = L"(() => { return function add(x, y) { return x + y; }; })()";
    IfFailCleanup(host.RunScript(szScript, L"", &result));

    // Get the function instance.apply
    JsPropertyIdRef applyId;
    IfFailCleanup(JsGetPropertyIdFromName(L"apply", &applyId));

    JsValueRef applyObj;
    IfFailCleanup(JsGetProperty(result, applyId, &applyObj));

    JsValueRef args;
    IfFailCleanup(host.RunScript(L"(() => [1,2])()", L"", &args));

    JsValueRef argsArray;
    IfFailCleanup(JsCreateArray(2, &argsArray));
    /*
    JsValueRef arg0, arg1;
    JsValueRef arg0Val, arg1Val;
    JsIntToNumber(0, &arg0);
    JsIntToNumber(1, &arg1);
    JsIntToNumber(42, &arg0Val);
    JsIntToNumber(56, &arg1Val);

    JsSetIndexedProperty(argsArray, arg0, arg0Val);
    JsSetIndexedProperty(argsArray, arg1, arg1Val);
    */
    JsValueRef nullObj;
    IfFailCleanup(JsGetNullValue(&nullObj));

    JsValueRef argObj[3] = { result , nullObj, args };
    JsValueRef returnObj;
    IfFailCleanup(JsCallFunction(applyObj, argObj, 3, &returnObj));

    JsValueRef stringObj;
    IfFailCleanup(JsConvertValueToString(returnObj, &stringObj));

    const wchar_t* szBuf;
    size_t bufLen;
    IfFailCleanup(JsStringToPointer(stringObj, &szBuf, &bufLen));

    wprintf(L"Result: %s", szBuf);
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
    return SerializeScript();
}

