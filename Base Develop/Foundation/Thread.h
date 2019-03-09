#pragma once

/*
	Thread Class (���� ��Ÿ��)

	1. ������ �켱���� ���� �� ����
	2. ������ ���� ���۱��
	3. ������ ���� ũ�� ���� -> inline���� �����ϰ� Start �� ������
	������ ���� ũ�� �����Ǿ� ����ȴ�.
	4. ������ �̸� ����
*/
#include "foundation.h"
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

	GET_SET_ATTRIBUTE(HANDLE, hthreadhandle);

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


inline void SetThreadName(HANDLE handle, const char* str)
{
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;        // must be 0x1000
		LPCSTR szName;       // pointer to name (in same addr space)
		DWORD dwThreadID;    // thread ID (-1 caller thread)
		DWORD dwFlags;       // reserved for future use, most be zero
	} THREADNAME_INFO;

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = str;
	info.dwThreadID = ::GetCurrentThreadId();
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD), (const ULONG_PTR *)&info);
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}