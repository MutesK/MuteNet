#pragma once

/*
	주로 모니터링 및 로그용도로 넣는다.

	cout 객체가 여러 스레드에서 동작하면 불안정하게 동작한다.
	이를 위해 정확하게 Console에서 동작하기위해서 만든 클래스

	현재 Not Working
*/
#include <concurrent_queue.h>

class ConsolePrinter : public Thread
{
public:
	ConsolePrinter();
	~ConsolePrinter();

	static ConsolePrinter* Instance();
	
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;
private:
	Concurrency::concurrent_queue<std::string>	_outputQueue;
	bool										_threadstoprequst;

	atomic<uint64_t>							_logcount;

	static std::once_flag						_once_call;
	static std::unique_ptr<ConsolePrinter>		_instance;
};

