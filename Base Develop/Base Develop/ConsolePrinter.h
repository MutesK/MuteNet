#pragma once

/*
	�ַ� ����͸� �� �α׿뵵�� �ִ´�.
	CSystemLog�� ������ ������.

	���� Not Working
	Task ����ý����� �̿ϼ��̴� �ٷ� ������� ó���Ѵ�.

	����� Console������ ����ϰ������� ���߿��� ���Ϸ�
	��¥����(��-��-��)/(�����̸�-��-��-��-��.log)�� ����Ѵ�.
*/

#include "ObjectPoolTLS.h"
#include "TaskManager.h"

enum Log_Level
{
	LOG_DEBUG = 0,
	LOG_WARNING,
	LOG_ERROR,
	LOG_SYSTEM,
};

enum
{
	LOG_MSGLEN_MAX = 10000
};

class ConsolePrinter : public Thread
{
public:
	ConsolePrinter();
	~ConsolePrinter();

	static ConsolePrinter* Instance();
	

	void Log(char *function, size_t line, const char* szStringFormat, ...);
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;
private:
	Concurrency::concurrent_queue<std::string>	_outputQueue;
	bool										_threadstoprequest;

	static std::once_flag						_once_call;
	static std::unique_ptr<ConsolePrinter>		_instance;

	std::atomic<uint64_t>						_logcount;
};
