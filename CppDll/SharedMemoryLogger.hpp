#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <windows.h>
#include "DateTime.hpp"

class SharedMemoryLogger
{
public:
	~SharedMemoryLogger()
	{
		CloseHandle(_hFile);
	}

	static SharedMemoryLogger& GetInstance()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_instance == nullptr) {
			_instance = new SharedMemoryLogger();
		}

		return *_instance;
	}

	void WriteLog(const char* szPipeUpdate)
	{
		DWORD dwWrite;

		if (_hFile == INVALID_HANDLE_VALUE) {
			DWORD dw = GetLastError();
			WriteErrorToFile("CreateFile failed for Named Pipe client\n:");
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

	void WriteLog(std::string msg)
	{
		//char buffer[500]{0};
		//int len = msg.size() > 500 ? 500 : msg.size();
		//memcpy(buffer, msg.c_str(), sizeof(char) * len);
		//this->WriteLog(buffer);
		this->WriteLog(msg.c_str());
	}

private:
	static SharedMemoryLogger* _instance;
	static std::mutex _mutex;
    HANDLE _hFile = NULL;

	SharedMemoryLogger()
	{
		_hFile = CreateFile((LPCSTR)"\\\\.\\pipe\\BvrPipe", GENERIC_WRITE,
				 0, NULL, OPEN_ALWAYS,
				 0, NULL);
	}

	void WriteErrorToFile(const char* message)
	{
		DateTime now = DateTime::Now();
		FILE* fp = 0;
		fopen_s(&fp, "./debug.txt", "a+");
		if (fp != 0)
		{
			fprintf(fp, "%s\t%s", now.ToString().c_str(), message);
			fclose(fp);
		}
	}
};

SharedMemoryLogger* SharedMemoryLogger::_instance = nullptr;
std::mutex SharedMemoryLogger::_mutex;