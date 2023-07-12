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

void WriteErrorToFile(const char* message);
void CreatePipeStream(const char* szPipeUpdate);
DLL_API int AddTwoNum(int a, int b);