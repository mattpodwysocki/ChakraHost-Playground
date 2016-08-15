#pragma once

#include <jsrt.h>

class ChakraHost
{
public:
	JsErrorCode Init();
	JsErrorCode Destroy();

	JsErrorCode CallModuleMethod(const wchar_t* szModule, const wchar_t* szMethod, JsValueRef* arguments, USHORT argumentsLength, JsValueRef* result);

	JsErrorCode RunScript(const wchar_t* szScript, const wchar_t* szSourceUri, JsValueRef* result);
	JsErrorCode RunScriptFromFile(const wchar_t* szFileName, const wchar_t* szSourceUri, JsValueRef* result);

	JsErrorCode JsonStringify(JsValueRef argument, JsValueRef* result);
	JsErrorCode JsonParse(JsValueRef argument, JsValueRef* result);

	JsErrorCode GetGlobalVariable(const wchar_t* szPropertyName, JsValueRef* result);
	JsErrorCode SetGlobalVariable(const wchar_t* szPropertyName, JsValueRef value);
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