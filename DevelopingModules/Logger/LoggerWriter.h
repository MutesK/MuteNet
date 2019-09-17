#pragma once

#include <queue>
#include <string>
#include <thread>
#include <shared_mutex>
#include <fstream>
#include <iostream>

class LoggerWriter
{
private:
	std::queue<std::string>  _Inputqueue;
	std::queue<std::string>  _writerqueue;

	std::shared_mutex		 _mutex;
	std::thread				 _writer;
	bool					 _signal = true;
public:
	LoggerWriter();
	~LoggerWriter();

	void FinializeLogger();
	void WriterTask();

private:
	void SwapQueue();
	void WriteLog();
};

