#pragma once

/*
	�ַ� ����͸� �� �α׿뵵�� �ִ´�.

	cout ��ü�� ���� �����忡�� �����ϸ� �Ҿ����ϰ� �����Ѵ�.
	�̸� ���� ��Ȯ�ϰ� Console���� �����ϱ����ؼ� ���� Ŭ����

	���� Not Working
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

