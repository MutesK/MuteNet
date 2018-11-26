#include "stdafx.h"
#include "Thread.h"
#include "ConsolePrinter.h"
#include <strsafe.h>

using namespace std;
std::unique_ptr<ConsolePrinter> ConsolePrinter::_instance;
once_flag ConsolePrinter::_once_call;

ConsolePrinter::ConsolePrinter()
{
	_logcount = 0;

	_threadstoprequest = false;
	std::string str = "ConsolePrinter";
	SetThreadName(str);

	Start();
}


ConsolePrinter::~ConsolePrinter()
{
}

ConsolePrinter* ConsolePrinter::Instance()
{
	std::call_once(_once_call,
		[&]()
	{
		_instance = std::make_unique<ConsolePrinter>();
	});

	return _instance.get();
}

void ConsolePrinter::DoWork()
{
	std::string outputStr;

	while (!_threadstoprequest)
	{
		_event.WaitSignal();

		while (!_outputQueue.empty())
		{
			if (_outputQueue.try_pop(outputStr))
			{
				// Console Print & File Print
				std::cout << outputStr;
			}
		}
	}
}

void ConsolePrinter::EmitWakeupSignal()
{
	_threadstoprequest = true;

	_event.SetEvent();
}


void ConsolePrinter::Log(char *function, size_t line, const char* szStringFormat, ...)
{
	char szLogMessage[LOG_MSGLEN_MAX];

	_logcount++;

	va_list va;
	va_start(va, szLogMessage);

	HRESULT hResult = StringCchPrintf(szLogMessage, LOG_MSGLEN_MAX, szStringFormat, va);
	va_end(va);

	if (FAILED(hResult))
	{
		// Exception;
		return;
	}

	std::string logString = szLogMessage;
	_outputQueue.push(logString);

	_event.SetEvent();
}