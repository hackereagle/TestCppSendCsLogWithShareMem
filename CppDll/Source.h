#pragma once

#ifdef DLL_EXPORT
#define DLL_API extern "C" __declspec(dllexport)
#else
#define DLL_API extern "C" __declspec(dllimport)
#endif // DLL_EXPORT

#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

DLL_API int AddTwoNum(int a, int b);