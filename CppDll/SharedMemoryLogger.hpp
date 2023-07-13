#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <windows.h>
#include <filesystem>
#include <fstream>
#include "DateTime.hpp"

//namespace fs = std::experimental::filesystem;

enum class LogLevel : int
{
	LOGTRACE,
	LOGINFO,
	LOGERROR,
	LOGFATAL,
};

class SharedMemoryLogger
{
public:
	~SharedMemoryLogger()
	{
		CloseHandle(_hTrace);
		CloseHandle(_hInfo);
		CloseHandle(_hError);
		CloseHandle(_hFatal);
		_fileStream.close();
	}

	static SharedMemoryLogger& GetInstance()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_instance == nullptr) {
			_instance = new SharedMemoryLogger();
		}

		return *_instance;
	}

	void WriteLog(LogLevel level, const char* szPipeUpdate)
	{
		DWORD dwWrite;

		HANDLE _hFile = GetLogLevelFileHandle(level);

		if (_hFile == INVALID_HANDLE_VALUE) {
			DWORD dw = GetLastError();
			std::ostringstream ss;
			ss << "CreateFile failed for Named Pipe client: " << dw << std::endl;
			WriteErrorToFile(ss.str().c_str());
		}
		else {
			size_t len = strlen(szPipeUpdate);
			BOOL ret = WriteFile(_hFile, szPipeUpdate, len, &dwWrite, NULL);
			if (FALSE == ret) {
				WriteErrorToFile("WriteFile failed for Named Pipe client\n");
			}
			else {
				WriteErrorToFile("WriteFile succeeded for Named Pipe client\n");
			}

			if (szPipeUpdate[len - 1] != '\n')
				WriteFile(_hFile, "\n", 1, &dwWrite, NULL);
		}
	}

	void WriteLog(LogLevel level, std::string msg)
	{
		this->WriteLog(level, msg.c_str());
	}

private:
	static SharedMemoryLogger* _instance;
	static std::mutex _mutex;
    HANDLE _hTrace = NULL;
    HANDLE _hInfo = NULL;
    HANDLE _hError = NULL;
    HANDLE _hFatal = NULL;
	std::fstream _fileStream;

	SharedMemoryLogger()
	{
		_fileStream.open("./debug.txt", std::ios::out | std::ios::trunc);
		_hTrace = CreateFile((LPCSTR)"\\\\.\\pipe\\CppInfoTrace", GENERIC_WRITE,
				 0, NULL, OPEN_ALWAYS,
				 0, NULL);
		_hInfo = CreateFile((LPCSTR)"\\\\.\\pipe\\CppInfoLog", GENERIC_WRITE,
				 0, NULL, OPEN_ALWAYS,
				 0, NULL);
		_hError = CreateFile((LPCSTR)"\\\\.\\pipe\\CppInfoError", GENERIC_WRITE,
				 0, NULL, OPEN_ALWAYS,
				 0, NULL);
		_hFatal = CreateFile((LPCSTR)"\\\\.\\pipe\\CppInfoFatal", GENERIC_WRITE,
				 0, NULL, OPEN_ALWAYS,
				 0, NULL);
	}

	void WriteErrorToFile(const char* message)
	{
		DateTime now = DateTime::Now();
		_fileStream << now.ToString() << "\t" << message << std::endl;
	}

	HANDLE GetLogLevelFileHandle(LogLevel level)
	{
		if (LogLevel::LOGINFO == level)
			return _hInfo;
		else if (LogLevel::LOGERROR == level)
			return _hError;
		else if (LogLevel::LOGTRACE == level)
			return _hTrace;
		else if (LogLevel::LOGFATAL == level)
			return _hFatal;
	}
};

SharedMemoryLogger* SharedMemoryLogger::_instance = nullptr;
std::mutex SharedMemoryLogger::_mutex;