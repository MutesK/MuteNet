#pragma once

#include "Logger.h"
#include "RotateLogger.h"

/*
 * Console Print와 File Print를 담당한다.
 */
class LoggerWriter
{
private:
	std::queue<std::string>  _Inputqueue;
	std::queue<std::string>  _writerqueue;

	std::shared_mutex		 _mutex;
	std::thread				 _writer;
	bool					 _signal = true;

	RotateLogger			 _rotater;
public:
	LoggerWriter();
	~LoggerWriter();

	void FinializeLogger();
private:
	void SwapQueue();
	void WriterTask();

	void WriteLog();
};

