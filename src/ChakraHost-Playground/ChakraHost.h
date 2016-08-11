#pragma once

#include <jsrt.h>

class ChakraHost
{
public:
	JsErrorCode Init();
	JsErrorCode Destroy();

private:
	JsErrorCode InitJsonParse();
	JsErrorCode InitConsole();

	unsigned currentSourceContext;
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef globalObject;
	JsValueRef jsonParseObject;
};