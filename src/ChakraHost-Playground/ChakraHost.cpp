#include "stdafx.h"
#include "ChakraHost.h"

JsErrorCode ChakraHost::Init()
{
	currentSourceContext = 0;
	JsErrorCode status = JsNoError;

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

	return status;
};

JsErrorCode ChakraHost::Destroy()
{
	JsErrorCode status = JsNoError;

	IfFailRet(JsSetCurrentContext(JS_INVALID_REFERENCE));
	IfFailRet(JsDisposeRuntime(runtime));

	return status;
}