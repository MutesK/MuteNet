#pragma once

#include "Logger.h"
#include "RotateLogger.h"

/*
 *	Console Print와 File Print를 담당한다.
 */
class LoggerWriter final
{
private:
	std::queue<std::string>  _Inputqueue;
	std::queue<std::string>  _writerqueue;

	std::shared_mutex		 _mutex;
	std::thread				 _writer;
	bool					 _signal = true;

	RotateLogger*			 _rotater;
	std::fstream			 _stream;
public:
	LoggerWriter(RotateLogger* Logger);
	~LoggerWriter();

	void EnqueueLog(const std::string& logString);
private:
	void InitializeFileStream();
	void FinializeLogger();
	void SwapQueue();
	void WriterTask();

	void WriteLog();

	void PrintConsole(const std::string& logString) const;
	void PrintFile(const std::string& logString);
};

