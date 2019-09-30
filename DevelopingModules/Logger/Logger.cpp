#include "Logger.h"
#include "StringFormat.h"
#include <cstdarg>

Logger::Logger(const std::string& AppName,
	const std::string& directory)
	:_logNo(0)
{
	_Rotater = std::make_unique<RotateLogger>();
	_Writer = std::make_unique<LoggerWriter>(_Rotater.get());

	_Rotater->Initialize(AppName, directory);
}

void Logger::Log(LogLevel level, char* stringFormat, ...)
{
	if (level < _loglevel)
		return;

	std::string LogString;

	va_list va;
	va_start(va, stringFormat);
	LogString = StringFormat::Format(stringFormat, va);
	va_end(va);

	_Writer->EnqueueLog(LogString);
}
