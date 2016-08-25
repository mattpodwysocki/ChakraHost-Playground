#pragma once

#include <jsrt.h>
#include <set>

JsErrorCode StringifyJsValue(JsValueRef value, USHORT depth, std::set<JsValueRef> seen);
JsErrorCode StringifyJsBoolean(JsValueRef value);
JsErrorCode StringifyJsString(JsValueRef value);
JsErrorCode StringifyJsFunction(JsValueRef value);
JsErrorCode StringifyJsObject(JsValueRef value, USHORT depth, std::set<JsValueRef> seen);
JsErrorCode StringifyJsArray(JsValueRef value, USHORT depth, std::set<JsValueRef> seen);
JsErrorCode StringifyJsTypedArray(JsValueRef value);
JsErrorCode StringifyToString(JsValueRef value);