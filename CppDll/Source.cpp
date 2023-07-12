#include "Source.h"
#include <tchar.h>
#include <iomanip>
#include "SharedMemoryLogger.hpp"

int AddTwoNum(int a, int b)
{
	int sum = 0;
    SharedMemoryLogger::GetInstance().WriteLog ("test");
	sum = a + b;
    SharedMemoryLogger::GetInstance().WriteLog ("yo");
	return sum;
}