#pragma once

/*
	주로 모니터링 및 로그용도로 넣는다.
	CSystemLog을 개편할 생각임.

	현재 Not Working
	Task 서브시스템이 미완성이니 바로 스레드로 처리한다.

	현재는 Console에서만 출력하고있지만 나중에는 파일로
	날짜폴더(년-월-일)/(서버이름-년-월-일-시.log)로 출력한다.
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
