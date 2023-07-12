#pragma once
#include <iostream>
#include <memory>

class SharedMemoryLogger
{
public:
	static SharedMemoryLogger& GetInstance()
	{
		if (_instance == nullptr) {
			_instance = std::make_unique<SharedMemoryLogger>();
		}

		return *_instance;
	}

private:
	static std::unique_ptr<SharedMemoryLogger> _instance = nullptr;

	SharedMemoryLogger()
	{
	}
};

static std::unique_ptr<SharedMemoryLogger> SharedMemoryLogger::_instance;