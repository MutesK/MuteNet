#include "LoggerWriter.h"
#include <algorithm>


LoggerWriter::LoggerWriter(RotateLogger* Rotater)
	:_rotater(Rotater)
{
	InitializeFileStream();
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

	_stream.close();
}

void LoggerWriter::EnqueueLog(const std::string& logString)
{
	std::unique_lock<std::shared_mutex> lock(_mutex);

	_writerqueue.push(logString);
}

void LoggerWriter::InitializeFileStream()
{
	while (true)
	{
		const auto path = _rotater->GetFullPath();

		_stream.open(path, std::ios::ate);
		const auto length = _stream.tellg();

		if (length >= 5000000)
		{
			_stream.close();
			_rotater->NextLogNumber();
			continue;
		}

		break;
	}
}

void LoggerWriter::WriterTask()
{
	while (_signal)
	{
		if (!_Inputqueue.empty())
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
	while (!_writerqueue.empty())
	{
		const auto& logStr = _writerqueue.front();

		PrintConsole(logStr);
		PrintFile(logStr);

		_writerqueue.pop();
	}
}

void LoggerWriter::PrintConsole(const std::string& logString) const
{
	std::cout << logString << std::endl;
}

void LoggerWriter::PrintFile(const std::string& logString)
{
	_stream << logString << std::endl;
}
