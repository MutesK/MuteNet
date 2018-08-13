#pragma once

/*
	Thread Class

	1. ������ �켱���� ���� �� ����
	2. ������ ���� ���۱��
	3. ������ ���� ũ�� ���� -> inline���� �����ϰ� Start �� ������
	������ ���� ũ�� �����Ǿ� ����ȴ�.
	4. ������ �̸� ����
*/
#include "Event.h"

class Thread
{
public:
	enum Priority
	{
		eTHREAD_HIGH,
		eTHREAD_NORMAL,
		eTHREAD_LOW,
	};

	Thread();
	virtual ~Thread();
	
	void Start();
	void Stop();

	void SetThreadName(const std::string& threadName);
	
	bool isThreadRunning();

	void SetEvent();

	void SetThreadID(uint32_t threadID);
protected:
	virtual void DoWork();
	virtual void EmitWakeupSignal() = 0; // ��ӹ��� Ŭ������ �����ƾ�� ó���ϰ� �Ѵ�.
private:
	static unsigned WINAPI ThreadProc(LPVOID arg);
	void SetThreadName(const char* strName);
private:
	HANDLE _hthreadhandle;
	DWORD  _hthreadId;
	std::string	_threadname;
	Priority _priority;
	uint32_t _stackSize;
protected:
	Event   _event;
	bool	_isthreadwork;
};

inline void Thread::SetThreadName(const std::string& threadName)
{
	_threadname = threadName;
}

inline void Thread::SetThreadID(uint32_t threadID)
{
	_hthreadId = threadID;
}

inline void Thread::SetEvent()
{
	_event.SetEvent();
}
