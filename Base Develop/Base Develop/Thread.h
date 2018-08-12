#pragma once

/*
	Thread Class

	1. 스레드 우선순위 설정 및 변경
	2. 스레드 중지 시작기능
	3. 스레드 스택 크기 조절 -> inline으로 설정하고 Start 할 시점에
	스레드 스택 크기 결정되어 실행된다.
	4. 스레드 이름 설정
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
	virtual void EmitWakeupSignal() = 0; // 상속받은 클래스가 종료루틴을 처리하게 한다.
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
