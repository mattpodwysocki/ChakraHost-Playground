#pragma once
  
#include <stdio.h>  
#include <stdarg.h>

int _aswprintf(wchar_t **ret, const wchar_t *format, ...);
int _asprintf(char **ret, const char *format, ...);