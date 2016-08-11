#include "stdafx.h"
#include "ChakraHost.h"

JsErrorCode ChakraHost::Init()
{
	JsErrorCode status = JsNoError;

	IfFailRet(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
	IfFailRet(JsCreateContext(runtime, &context));
	IfFailRet(JsSetCurrentContext(context));

	return status;
};

JsErrorCode ChakraHost::Destroy()
{
	JsErrorCode status = JsNoError;

	IfFailRet(JsSetCurrentContext(JS_INVALID_REFERENCE));
	IfFailRet(JsDisposeRuntime(runtime));

	return status;
}