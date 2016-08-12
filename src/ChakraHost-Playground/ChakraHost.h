#pragma once

#include <jsrt.h>

class ChakraHost
{
public:
	JsErrorCode Init();
	JsErrorCode Destroy();

	JsErrorCode RunScript(const wchar_t* szScript, const wchar_t* szSourceUri, JsValueRef* result);
private:
	JsErrorCode InitJson();
	JsErrorCode InitConsole();

	unsigned currentSourceContext;
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef globalObject;
	JsValueRef jsonParseObject;
	JsValueRef jsonStringifyObject;
};