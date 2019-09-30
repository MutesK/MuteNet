#pragma once

#include <atomic>
#include <string>
#include <queue>
#include <thread>
#include <shared_mutex>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_map>
#include "LoggerWriter.h"

enum LogLevel
{
	DEBUG = 0,
	WARNING,
	ERROR,
	SYSTEM
};

class Logger final
{
private:
	int					_loglevel = LogLevel::DEBUG;
	size_t				_logNo;
	
	std::unique_ptr<LoggerWriter>  _Writer;
	std::unique_ptr<RotateLogger>  _Rotater;
public:
	Logger(const std::string& AppName,
		const std::string& directory);

	void SetLogLevel(LogLevel level);

	void Log(LogLevel level, char* stringFormat, ...);
};

inline void Logger::SetLogLevel(LogLevel level)
{
	_loglevel = level;
}
