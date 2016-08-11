// ChakraHost-Playground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChakraHost.h"

int main()
{
	ChakraHost host;

	if (host.Init() != JsNoError)
	{
		wprintf(L"Error in initializing runtime");
		return -1;
	}

	wprintf(L"Runtime initialized");

	if (host.Destroy() != JsNoError)
	{
		wprintf(L"Error in destroying runtime");
		return -1;
	}

    return 0;
}

