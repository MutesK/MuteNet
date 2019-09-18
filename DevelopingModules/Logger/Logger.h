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
	static const size_t LOG_LEN = 10000;

	std::atomic_int		_loglevel = LogLevel::DEBUG;
	size_t				_logNo;
	
	LoggerWriter		_writer;
public:
	Logger() = default;
	~Logger() = default;

	void SetLogLevel(LogLevel level);
	void SetLogDirectory(std::string directory);

	void Log(LogLevel level, char* stringFormat, ...);
};


inline void Logger::SetLogLevel(LogLevel level)
{
	_loglevel = level;
}




