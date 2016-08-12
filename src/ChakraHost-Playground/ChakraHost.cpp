#include "stdafx.h"
#include "ChakraHost.h"

void ThrowException(const wchar_t* szException)
{
	// We ignore error since we're already in an error state.
	JsValueRef errorValue;
	JsValueRef errorObject;
	JsPointerToString(szException, wcslen(szException), &errorValue);
	JsCreateError(errorValue, &errorObject);
	JsSetException(errorObject);
}

JsErrorCode DefineHostCallback(JsValueRef globalObject, const wchar_t *callbackName, JsNativeFunction callback, void *callbackState)
{
	JsPropertyIdRef propertyId;
	IfFailRet(JsGetPropertyIdFromName(callbackName, &propertyId));

	JsValueRef function;
	IfFailRet(JsCreateFunction(callback, callbackState, &function));
	IfFailRet(JsSetProperty(globalObject, propertyId, function, true));

	return JsNoError;
};

JsValueRef InvokeConsole(const wchar_t* kind, JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	ChakraHost* self = (ChakraHost*)callbackState;
	wprintf(L"[JS {%s}]", kind);

	for (USHORT i = 1; i < argumentCount; i++)
	{
		JsValueRef arg = arguments[i];
		JsValueType type;
		IfFailThrow(JsGetValueType(arg, &type), L"Cannot get the value type for the parameter");

		switch (type)
		{
			case JsUndefined:
				wprintf(L"undefined");
				break;
			case JsNull:
				wprintf(L"null");
				break;
			case JsBoolean:
				bool bBool;
				JsBooleanToBool(arg, &bBool);
				wprintf(bBool ? L"true" : L"false");
				break;
			case JsNumber:
			case JsString:
				JsValueRef jsArgs[1] = { arg };
				JsValueRef jsResult;
				IfFailThrow(self->JsonStringify(jsArgs, &jsResult), L"JSON.stringify failed");
				
				wchar_t* szBuf = NULL;
				size_t szBufLen;
				IfFailThrow(JsStringToPointer(jsResult, &szBuf, &szBufLen), L"Failed to get string from JSON.stringify");
				wprintf(szBuf);
				delete[] szBuf;
				break;
			default:
				break;
		}

		wprintf(L" ");
	}

	wprintf(L"\n");

	return JS_INVALID_REFERENCE;
};

JsValueRef CALLBACK ConsoleLog(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return InvokeConsole(L"log", callee, isConstructCall, arguments, argumentCount, callbackState);
};

JsValueRef CALLBACK ConsoleWarn(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return InvokeConsole(L"warn", callee, isConstructCall, arguments, argumentCount, callbackState);
};

JsValueRef CALLBACK ConsoleInfo(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return InvokeConsole(L"info", callee, isConstructCall, arguments, argumentCount, callbackState);
};

JsValueRef CALLBACK ConsoleError(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return InvokeConsole(L"error", callee, isConstructCall, arguments, argumentCount, callbackState);
};

JsErrorCode ChakraHost::JsonStringify(JsValueRef* arguments, JsValueRef* result)
{
	return JsCallFunction(jsonStringifyObject, arguments, 1, result);
};

JsErrorCode ChakraHost::InitJson()
{
	JsPropertyIdRef jsonPropertyId;
	IfFailRet(JsGetPropertyIdFromName(L"JSON", &jsonPropertyId));
	JsValueRef jsonObject;
	IfFailRet(JsGetProperty(globalObject, jsonPropertyId, &jsonObject));
	
	JsPropertyIdRef jsonParseId;
	IfFailRet(JsGetPropertyIdFromName(L"parse", &jsonParseId));
	IfFailRet(JsGetProperty(jsonObject, jsonParseId, &jsonParseObject));

	JsPropertyIdRef jsonStringifyId;
	IfFailRet(JsGetPropertyIdFromName(L"stringify", &jsonStringifyId));
	IfFailRet(JsGetProperty(jsonObject, jsonStringifyId, &jsonStringifyObject));

	return JsNoError;
};

JsErrorCode ChakraHost::InitConsole()
{
	JsPropertyIdRef consolePropertyId;
	IfFailRet(JsGetPropertyIdFromName(L"console", &consolePropertyId));

	JsValueRef consoleObject;
	IfFailRet(JsCreateObject(&consoleObject));
	IfFailRet(JsSetProperty(globalObject, consolePropertyId, consoleObject, true));

	IfFailRet(DefineHostCallback(consoleObject, L"info", ConsoleInfo, this));
	IfFailRet(DefineHostCallback(consoleObject, L"log", ConsoleLog, this));
	IfFailRet(DefineHostCallback(consoleObject, L"warn", ConsoleWarn, this));
	IfFailRet(DefineHostCallback(consoleObject, L"error", ConsoleError, this));

	return JsNoError;
};

JsErrorCode ChakraHost::Init()
{
	currentSourceContext = 0;

	IfFailRet(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
	IfFailRet(JsCreateContext(runtime, &context));
	IfFailRet(JsSetCurrentContext(context));
	
	IfFailRet(JsGetGlobalObject(&globalObject));

	IfFailRet(InitJson());
	IfFailRet(InitConsole());

	return JsNoError;
};

JsErrorCode ChakraHost::Destroy()
{
	IfFailRet(JsSetCurrentContext(JS_INVALID_REFERENCE));
	IfFailRet(JsDisposeRuntime(runtime));

	return JsNoError;
};