#include "stdafx.h"
#include "Thread.h"
#include "ConsolePrinter.h"

using namespace std;
std::unique_ptr<ConsolePrinter> ConsolePrinter::_instance;
once_flag ConsolePrinter::_once_call;

ConsolePrinter::ConsolePrinter()
{
	_logcount = 0;

	_threadstoprequst = false;
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

	while (!_threadstoprequst)
	{
		while (!_outputQueue.empty())
		{
			if (!_outputQueue.try_pop(outputStr))
				continue;

			std::cout << outputStr;
		}
	}
}

void ConsolePrinter::EmitWakeupSignal()
{
	_threadstoprequst = true;
}
