#pragma once

#include <jsrt.h>

class ChakraHost
{
public:
	JsErrorCode Init();
	JsErrorCode Destroy();

	JsErrorCode RunScript(const wchar_t* szScript, const wchar_t* szSourceUri, JsValueRef* result);
	JsErrorCode RunScriptFromFile(const wchar_t* szFileName, const wchar_t* szSourceUri, JsValueRef* result);
	JsErrorCode JsonStringify(JsValueRef argument, JsValueRef* result);
	JsErrorCode JsonParse(JsValueRef argument, JsValueRef* result);
private:
	JsErrorCode InitJson();
	JsErrorCode InitConsole();
	JsErrorCode InitRequire();

	unsigned currentSourceContext;
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef globalObject;
	JsValueRef requireObject;
	JsValueRef jsonParseObject;
	JsValueRef jsonStringifyObject;
};