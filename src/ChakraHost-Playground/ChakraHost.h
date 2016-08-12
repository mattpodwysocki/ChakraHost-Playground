#pragma once

#include <jsrt.h>

class ChakraHost
{
public:
	JsErrorCode Init();
	JsErrorCode Destroy();

	JsErrorCode JsonStringify(JsValueRef* arguments, JsValueRef* result);

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