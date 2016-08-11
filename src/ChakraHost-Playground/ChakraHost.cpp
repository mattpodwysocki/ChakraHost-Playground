#include "stdafx.h"
#include "ChakraHost.h"

JsErrorCode DefineHostCallback(JsValueRef globalObject, const wchar_t *callbackName, JsNativeFunction callback, void *callbackState)
{
	JsPropertyIdRef propertyId;
	IfFailRet(JsGetPropertyIdFromName(callbackName, &propertyId));

	JsValueRef function;
	IfFailRet(JsCreateFunction(callback, callbackState, &function));

	IfFailRet(JsSetProperty(globalObject, propertyId, function, true));

	return JsNoError;
};

JsValueRef CALLBACK ConsoleLog(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return JS_INVALID_REFERENCE;
};

JsValueRef CALLBACK ConsoleWarn(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return JS_INVALID_REFERENCE;
};

JsValueRef CALLBACK ConsoleInfo(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return JS_INVALID_REFERENCE;
};

JsValueRef CALLBACK ConsoleError(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	return JS_INVALID_REFERENCE;
};

JsErrorCode ChakraHost::Init()
{
	currentSourceContext = 0;

	IfFailRet(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
	IfFailRet(JsCreateContext(runtime, &context));
	IfFailRet(JsSetCurrentContext(context));
	
	IfFailRet(JsGetGlobalObject(&globalObject));

	// Set up the console
	JsPropertyIdRef consolePropertyId;
	IfFailRet(JsGetPropertyIdFromName(L"console", &consolePropertyId));

	JsValueRef consoleObject;
	IfFailRet(JsCreateObject(&consoleObject));
	IfFailRet(JsSetProperty(globalObject, consolePropertyId, consoleObject, true));

	IfFailRet(DefineHostCallback(consoleObject, L"info", ConsoleInfo, nullptr));
	IfFailRet(DefineHostCallback(consoleObject, L"log", ConsoleLog, nullptr));
	IfFailRet(DefineHostCallback(consoleObject, L"warn", ConsoleWarn, nullptr));
	IfFailRet(DefineHostCallback(consoleObject, L"error", ConsoleError, nullptr));

	return JsNoError;
};

JsErrorCode ChakraHost::Destroy()
{
	IfFailRet(JsSetCurrentContext(JS_INVALID_REFERENCE));
	IfFailRet(JsDisposeRuntime(runtime));

	return JsNoError;
};