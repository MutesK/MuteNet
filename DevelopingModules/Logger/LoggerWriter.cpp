#include "LoggerWriter.h"
#include <algorithm>

LoggerWriter::LoggerWriter()
{
}

LoggerWriter::~LoggerWriter()
{
	FinializeLogger();
}

void LoggerWriter::FinializeLogger()
{
	if (_writer.joinable())
	{
		_signal = false;
		_writer.join();
	}
}

void LoggerWriter::WriterTask()
{
	while (_signal)
	{
		if (_Inputqueue.size() <= 0)
			continue;

		SwapQueue();
		WriteLog();
	}
}

void LoggerWriter::SwapQueue()
{
	std::unique_lock<std::shared_mutex> lock(_mutex);

	_Inputqueue.swap(_writerqueue);
}

void LoggerWriter::WriteLog()
{
	std::shared_lock<std::shared_mutex> lock(_mutex);

	FILE* fp = nullptr;
	std::string location;

	while (_writerqueue.size() > 0)
	{
		const auto& logStr = _writerqueue.front();

		std::cout << logStr;
		_writerqueue.pop();
	}
}
