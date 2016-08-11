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

#define IfFailRet(v) \
    { \
        JsErrorCode error = (v); \
        if (error != JsNoError) \
        { \
            return error; \
        } \
    }

// TODO: reference additional headers your program requires here
