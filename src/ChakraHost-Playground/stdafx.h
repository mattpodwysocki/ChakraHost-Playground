// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <windows.h>
#define USE_EDGEMODE_JSRT
#include <jsrt.h>
#include <stdio.h>
#include <tchar.h>

#define IfFailCleanup(v) \
    { \
        status = (v); \
        if (status != JsNoError) \
        { \
            goto cleanup; \
        } \
    }

#define IfFailRet(v) \
    { \
        JsErrorCode error = (v); \
        if (error != JsNoError) \
        { \
            return error; \
        } \
    }

#define IfFailThrow(v, e) \
    { \
        JsErrorCode error = (v); \
        if (error != JsNoError) \
        { \
            ThrowException((e)); \
            return JS_INVALID_REFERENCE; \
        } \
    }
