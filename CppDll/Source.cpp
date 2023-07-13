#include <tchar.h>
#include <iomanip>
#include <string>
#include <sstream>
#include "Source.h"
#include "SharedMemoryLogger.hpp"

int AddTwoNum(int a, int b)
{
    SharedMemoryLogger::GetInstance().WriteLog (LogLevel::LOGINFO, "Enter cpp function AddTwoNum");

	int sum = 0;
	sum = a + b;

	std::ostringstream ss;
	ss << a << " + " << b << " = " << sum << std::endl;
	SharedMemoryLogger::GetInstance().WriteLog(LogLevel::LOGINFO, ss.str());

    SharedMemoryLogger::GetInstance().WriteLog (LogLevel::LOGINFO, "Finished function work.");
	return sum;
}

void WriteErrorLog()
{
    SharedMemoryLogger::GetInstance().WriteLog (LogLevel::LOGERROR, "Test write LOGERROR success!");
}