#include <tchar.h>
#include <iomanip>
#include <string>
#include <sstream>
#include "Source.h"
#include "SharedMemoryLogger.hpp"

int AddTwoNum(int a, int b)
{
    SharedMemoryLogger::GetInstance().WriteLog ("Enter cpp function AddTwoNum");

	int sum = 0;
	sum = a + b;

	std::ostringstream ss;
	ss << a << " + " << b << " = " << sum << std::endl;
	SharedMemoryLogger::GetInstance().WriteLog(ss.str());

    SharedMemoryLogger::GetInstance().WriteLog ("Finished function work.");
	return sum;
}