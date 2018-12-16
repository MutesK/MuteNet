#pragma once

/*
	�ַ� ����͸� �� �α׿뵵�� �ִ´�.
	CSystemLog�� ������ ������.

	���� Not Working
	Task ����ý����� �̿ϼ��̴� �ٷ� ������� ó���Ѵ�.

	����� Console������ ����ϰ������� ���߿��� ���Ϸ�
	��¥����(��-��-��)/(�����̸�-��-��-��-��.log)�� ����Ѵ�.
*/

#include "TaskManager.h"

enum class Log_Level
{
	LOG_DEBUG = 0,
	LOG_WARNING,
	LOG_ERROR,
	LOG_SYSTEM,
};


#include "Singleton.hpp"

class ConsolePrinter : public Singleton<ConsolePrinter>, Thread
{
public:
	ConsolePrinter();
	~ConsolePrinter();

	void Log(char *function, size_t line, const char* szStringFormat, ...);

	GET_SET_ATTRIBUTE(size_t, logLevel);
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;
private:
	enum
	{
		LOG_MSGLEN_MAX = 10000
	};

	Concurrency::concurrent_queue<std::string>	_outputQueue;
	bool										_threadstoprequest;

	static std::once_flag						_once_call;
	static std::unique_ptr<ConsolePrinter>		_instance;

	std::atomic<uint64_t>						_logcount;
	std::atomic<size_t>							_logLevel;
};
