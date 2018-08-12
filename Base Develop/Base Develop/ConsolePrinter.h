#pragma once

/*
	�ַ� ����͸� �� �α׿뵵�� �ִ´�.
	CSystemLog�� ������ ������.

	���� Not Working
	Task ����ý����� �̿ϼ��̴� �ٷ� ������� ó���Ѵ�.

	����� Console������ ����ϰ������� ���߿��� ���Ϸ�
	��¥����(��-��-��)/(�����̸�-��-��-��-��.log)�� ����Ѵ�.
*/


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

	std::atomic<uint64_t>							_logcount;

	static std::once_flag						_once_call;
	static std::unique_ptr<ConsolePrinter>		_instance;
};

