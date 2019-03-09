
#include "Thread.h"

Thread::Thread()
	:_hthreadhandle(0), _threadname(""),
	_priority(eTHREAD_NORMAL), _stackSize(16111),
	_isthreadwork(false)
{
}


Thread::~Thread()
{
}

void Thread::Start()
{
	// ������ ����
	_hthreadhandle = (HANDLE)_beginthreadex(nullptr, _stackSize, 
		ThreadProc, this, CREATE_SUSPENDED, nullptr);

	// �켱 ���� ����
	switch (_priority)
	{
	case eTHREAD_HIGH:
		SetThreadPriority(_hthreadhandle, THREAD_PRIORITY_HIGHEST);
		break;
	case eTHREAD_NORMAL:
		SetThreadPriority(_hthreadhandle, THREAD_PRIORITY_NORMAL);
		break;
	case eTHREAD_LOW:
		SetThreadPriority(_hthreadhandle, THREAD_PRIORITY_LOWEST);
		break;
	}

	_isthreadwork = true;

	_hthreadId = GetThreadId(_hthreadhandle);
	_event.SetEventName(_threadname + "event");
	_event.SetResetFlag(false);
	_event.CreateEvent();

	// Resume Thread
	ResumeThread(_hthreadhandle);

}

void Thread::Stop()
{

	// ������ ���������� üũ
	if (!_isthreadwork)
		return;

	EmitWakeupSignal();
	
	WaitForSingleObject(_hthreadhandle, INFINITE);

	CloseHandle(_hthreadhandle);
}


void Thread::SetThreadName(const char* strName)
{
	_threadname = strName;

	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;        // must be 0x1000
		LPCSTR szName;       // pointer to name (in same addr space)
		DWORD dwThreadID;    // thread ID (-1 caller thread)
		DWORD dwFlags;       // reserved for future use, most be zero
	} THREADNAME_INFO;

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = _threadname.c_str();
	info.dwThreadID = _hthreadId;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD), (const ULONG_PTR *)&info);
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}

UINT WINAPI Thread::ThreadProc(LPVOID arg)
{
	Thread *pThread = reinterpret_cast<Thread *>(arg);
	// ������ ID ����
	// ������ �̸� ����
	pThread->SetThreadName(pThread->_threadname.c_str());
	// �̺�Ʈ �ñ׳� �ش�. -> ���� �̺�Ʈ ��ó����.
	pThread->SetEvent();
	pThread->DoWork();
	return 0;
}

// This function is abstract function.
void Thread::DoWork()
{
}

bool Thread::isThreadRunning()
{
	return _isthreadwork;
}

